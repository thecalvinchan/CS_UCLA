export PATH=/usr/local/cs/bin:$PATH

./alford.bash
./bolden.bash
./hamilton.bash
./powell.bash
./parker.bash

echo "IAMAT kiwi.cs.ucla.edu +27.5916+086.5640 `date +%s`" | telnet localhost 12590
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12590
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12591
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12592
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12593
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12594

echo "PEER powell tcp localhost 12593" | telnet localhost 12590
echo "PEER parker tcp localhost 12594" | telnet localhost 12590
echo "PEER powell tcp localhost 12593" | telnet localhost 12591
echo "PEER parker tcp localhost 12594" | telnet localhost 12591
echo "PEER parker tcp localhost 12594" | telnet localhost 12592

echo "IAMAT kiwi.cs.ucla.edu +27.5916+086.5640 `date +%s`" | telnet localhost 12590
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12590
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12591
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12592
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12593
echo "WHATSAT kiwi.cs.ucla.edu 100 2" | telnet localhost 12594
