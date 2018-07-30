# hamiltonSolver
## Application
This application checks whether a given graph (in DIMACS format) has an [hamiltonian cycle](https://en.wikipedia.org/wiki/Hamiltonian_path). Therefore, the problem is encoded in a boolean satisfiability problem and solved by a SAT solver. If an hamiltonian cycle exists, it is printed. This software was developed by A. Huber, C. Möller and A. Shulga as a project during the lecture Forschungslinie at the TU Dresden in the summer of 2016.

## Usage
> ./hamiltonSolver *graphFile.col*

If the file was a valid graph, either "UNSATISFIABLE" or "SATISFIABLE" and the hamiltonian cycle will be printed in a newly created output.txt. A few graphs are included in this repository.

## Requirements
- [riss64](http://tools.computational-logic.org/content/riss.php) in path, or edit call of riss64 in main function
- developed and tested in Ubuntu 16.04 and OS X