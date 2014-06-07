#!/usr/bin/python

import time
import datetime
import sys
import re
import json

from twisted.internet.protocol import Factory, ClientFactory, ReconnectingClientFactory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor
from twisted.internet.defer import Deferred
from twisted.internet.endpoints import TCP4ClientEndpoint, TCP4ServerEndpoint
from twisted.python import log
from twisted.internet.protocol import Protocol
from twisted.web.http_headers import Headers
from twisted.web.client import Agent, getPage
from twisted.python.util import println

GOOGLE_PLACES_API_KEY = "AIzaSyAAagfD-IjXLpZ4IflXi9ngnq8gpK1sAqs"

def sendPeer(protocol, message_to_pass):
    protocol.send_message(message_to_pass)

class ServerProtocol(LineReceiver):
    def __init__(self, factory):
        self.factory = factory

    def lineReceived(self, message):
        self.factory.logInfo('Received message: {0}'.format(message))
        if message.startswith('IAMAT'):
            self.IAMAT(message)
        elif message.startswith('WHATSAT'):
            self.WHATSAT(message)
        elif message.startswith('PEER'):
            self.PEER(message)
        else:
            self.handleError(message)
    
                
    def IAMAT(self, message):
        if len(message.split()) != 4:
            self.factory.logError('IAMAT: invalid num of args {0}'.format(message))
            self.handleError(message)
            return
        command, client, location, posixTime = message.split()
        try:
            posixTime = float(posixTime)
            clientTime = datetime.datetime.utcfromtimestamp(posixTime)
        except Exception as e:
            self.factory.logError('IAMAT: invalid time {0}'.format(posixTime))
            self.handleError(message)
            return
        systemTime = datetime.datetime.utcnow()
        timeDiff = systemTime - clientTime
        self.factory.users[client] = (location, clientTime, systemTime)
        locationMessage = 'AT {0} {1!r} {2}'.format(self.factory.server_name, timeDiff.total_seconds(), ' ') + client + str(location) + ' ' + str(posixTime)
        self.sendMessage(locationMessage)
        self.transport.loseConnection()
        self.factory.logInfo('Num of peers: {0}'.format(len(self.factory.peers.keys())))
        for peer in self.factory.peers.keys():
            self.factory.logInfo('Sending location information to peer: {0}'.format(peer))
            peerInfo = self.factory.peers[peer]
            peer, protocol, host, port = peerInfo
            self.factory.logInfo('Peer info: {0} {1} {2} {3}'.format(peer, protocol, host, port))
            end_point = None
            if protocol == 'tcp':
                end_point = TCP4ClientEndpoint(reactor, host, port)
                self.factory.logInfo('Endpoint created: {0!s}'.format(end_point))
                self.factory.logInfo('Connecting to peer: {0} {1} {2} {3}'.format(peer, protocol, host, port))
                connection = end_point.connect(self)
                connection.addCallback(sendPeer, locationMessage)

    def WHATSAT(self, message):
        if len(message.split()) != 4:
            self.factory.logError('WHATSAT: invalid num of args {0}'.format(message))
            self.handleError(message)
            return
        command, client, radius, maxPlaces = message.split()
        try:
            radius = float(radius)
            maxPlaces = int(maxPlaces)
        except Exception as e:
            self.factory.logError('WHATSAT command has malformed args: {0}'.format(message))
            self.handleError(message)
            return
        if not self.factory.users.has_key(client):
            self.factory.logError('Unable to find user: {0}'.format(client))
            self.handleError(message)
            return
        clientInfo = self.factory.users[client]
        location, clientTime, systemTime = clientInfo
        p = re.compile("[+-]")
        longitude = p.search(location[1:]).start()+1
        location = location[0:longitude] + "," + location[longitude:]
        timeDiff = systemTime -clientTime
        self.sendMessage('AT {0} {1!r} {2} {3} {4}'.format(self.factory.server_name, timeDiff.total_seconds(), client, str(location), str(clientTime)))
        self.findPlaces(location, radius, maxPlaces)

    def PEER(self, message):
        tokens = message.split()
        if not len(tokens) == 5:
            self.factory.logError('PEER command malformed: {0}'.format(message))
            self.handleError(message)
            return
        command, peer, protocol, host, port = tokens
        try:
            port = int(port)
        except Exception as e:
            self.factory.logError('PEER command has malformed port num: {0}'.format(command))
            self.handleError(command)
            return
        if self.factory.peers.has_key(peer):
            self.factory.logInfo('Peer already exists: {0}'.format(peer))
        else:
            self.factory.peers[peer] = (peer, protocol, host, port)
            self.factory.logInfo('Connecting with peer: {0}'.format(peer))
            end_point = None
            if protocol == 'tcp':
                end_point = TCP4ServerEndpoint(reactor, host, port)
                self.factory.logInfo('Endpoint created: {0!s}'.format(end_point))
                self.factory.logInfo('Connecting to peer: {0} {1} {2} {3}'.format(peer, protocol, host, port))
                connection = end_point.listen(self)
                connection.addCallback(sendPeer, 'PEER {0} tcp {1} {2}'.format(self.factory.server_name, self.factory.host_name, self.factory.port_number))
        self.factory.logInfo('Num of peers: {0}'.format(len(self.peers.keys())))

    def handleError(self, command):
        self.sendMessage('?: {0}'.format(command))
        self.transport.loseConnection()
        
    def sendMessage(self, message):
        self.sendLine(message)
        self.factory.logInfo('Sent message: {0}'.format(message))
        
    def findPlaces(self, client_location, radius_km, maxPlaces):
        print(client_location)
        url = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?key="+GOOGLE_PLACES_API_KEY+"&sensor=false"
        url += "&location=" + client_location
        url += '&radius=' + str(radius_km)
        getPage(url).addCallbacks(lambda places: self.gotPlaces(places, maxPlaces))

    def gotPlaces(self, value, maxPlaces):
        obj = json.loads(value)
        print obj
        obj[u'results'] = obj[u'results'][0:maxPlaces]
        obj = json.dumps(obj, indent=4, separators=(',',': '))
        self.sendMessage(obj)
        self.transport.loseConnection()
            
    def connectionMade(self):
        self.factory.logInfo('Connection made: {0}'.format(self.transport.getHost()))

    def connectionLost(self, reason):
        self.factory.logInfo('Connection lost.')




class Server(Factory):

    def __init__(self, server_name, host_name, port_number):
        self.server_name = server_name
        self.host_name = host_name
        self.port_number = port_number
        self.users = {}
        self.peers = {}
        self.log_stream = open('{0}_{1}.log'.format(self.server_name, datetime.datetime.utcnow().isoformat().replace(':', '_').replace('T', '_')), 'w')
        self.logInfo('Server opening.')

    def stopFactory(self):
        self.logInfo('Server closing.')
        self.log_stream.close()

    def buildProtocol(self, addr):
        self.logInfo('Protocol built for address: {0!s}'.format(addr))
        return ServerProtocol(self)

    def logInfo(self, message):
        self._log('INFO: {0!s}'.format(message))

    def logError(self, message):
        self._log('ERROR: {0!s}'.format(message))

    def _log(self, message):
        message = '{0}: {1}: {2}'.format(self.server_name, datetime.datetime.utcnow().isoformat(), message)
        print(message)
        try:
            self.log_stream.write(message+'\n')
        except ValueError as e:
            print('Could not log to file, file stream has been closed.')
            
if __name__ == '__main__':
    if(len(sys.argv) != 4):
        print "Incorrect num of command line arguemnts"
        exit()
    server = Server(sys.argv[1], sys.argv[2], int(sys.argv[3]))
    reactor.listenTCP(int(sys.argv[3]), server)
    reactor.run()
