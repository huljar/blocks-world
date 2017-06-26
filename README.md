# Blocks World - use SAT solving for planning problems

This is a small C++ tool to convert a description of the "blocks world" into propositional logic formulas. The formulas are then fed to a SAT solver (*miniSAT* currently) in Dimacs CNF format. The SAT solver will attempt to solve the problem; if a solution is found, it is extracted from miniSAT's output and printed.

## Problem descriptions

In the blocks world, you have a table and a number of blocks stacked on top of each other. You start with an initial state (how the blocks are currently stacked) and specify a goal state (how you want the blocks to be stacked). In addition, you give a number of time steps in which you want the goal state to be reached. During each time step, one block can be moved.

### Example

```
     Initial state    --->    Goal state

                                 ___
                                |   |
                                | a |
   ___                          |___|
  |   |                         |   |
  | b |                         | c |
  |___|    ___                  |___|
  |   |   |   |                 |   |
  | a |   | c |                 | b |
 _|___|___|___|_____       _____|___|_________
|__   _________   __|     |__   _________   __|
   | |         | |           | |         | |
   | |         | |           | |         | |
   | |         | |           | |         | |
   |_|         |_|           |_|         |_|
```

The call to `blocks` would be:

```
./blocks "a,b c" "b,c,a" 4
```

to attempt to find a solution within 4 time steps. 4 is the minimum required time for the above problem (the initial state counts as step 1), with smaller values the problem is unsatisfiable.

## miniSAT

The program currently requires the miniSAT binary to be placed at */usr/bin/minisat*.

## Memory

The code is not optimized at all right now. If you specify small problems (around 5-6 blocks and <15 time steps), it works, but with larger problems you will find yourself running out of memory very quickly. Maybe this will change in the future.

## License

The code is licensed under the MIT license. For more information, please read the [LICENSE.md](LICENSE.md) file.
