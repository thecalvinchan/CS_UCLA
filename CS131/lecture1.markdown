`tr -cs A-Za-z '[\n]*' | sort | uniq -c | sort -rn`

##Sapir Wharf Hypothesis

1. The language we use to some extent determines how we view the world and how we think
2. The structural diversity of languages is essentially limitless

##This course's core

- Principles and limitations of programming models
- Notations for these models: design + use + support
- Methods to evaluate strengths and weaknesses in various contexts

Read Ch 1-3,5,7,9

##Judging Languages

- portability (platform choice and ease of moving to another platform)
- library support
- speed of writing code (writability)
- speed of reading code (readability)
- documentation
- scalability (development/production)
- reliability
- modularity/abstraction
- speed of execution
- speed of compilation
- licensing fees

##Language Design Issue

- Orthogonality: no special cases when choosing features
    - functions in C can return any type except arrays
    - arrays can be large, returning them caould be slow
- Efficiency
    - CPU, RAM, I/O, disk access, flash, network access, power
- Scalability
- Simplicity
- Convenience
- Safety
    - compile-time checking
        - static and safe
    - run-time checking
        - dynamic and flexible
    - type checking
    - NULL pointers
    - out of bounds arrays
- Concurrency
- Mutability
    - How well do programs evolve?
    - How well do languages evolve?
