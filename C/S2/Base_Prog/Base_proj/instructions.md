### Project Specification: `treestat`

**Objective:** Develop a CLI utility in C for POSIX systems that performs recursive directory traversal to visualize the hierarchy and provide low-level metadata analytics.

---

### Technical Requirements

* **Language:** C (C99 or C11 standard).
* **Compiler Flags:** `-Wall -Wextra -Werror`.
* **Environment:** Linux/POSIX (standard libraries only; no external "tree" libraries).

---

### Phase 1: Core Traversal & Visualization

**Goal:** Implement a depth-first search (DFS) to print the directory structure.

* **Task 1.1:** Use `opendir()`, `readdir()`, and `closedir()` to navigate the filesystem.
* **Task 1.2:** Implement recursion to handle nested subdirectories.
* **Task 1.3:** Formatting: Print indentation levels to represent depth (e.g., using `│   ` and `├── `).
* **Constraint:** You must filter out the `.` and `..` entries to avoid infinite loops.

---

### Phase 2: Metadata Extraction & Logic

**Goal:** Integrate `stat` system calls to distinguish between different inode types and sizes.

* **Task 2.1:** Use `lstat()` for every entry. You must use `lstat` instead of `stat` to prevent the program from automatically following symbolic links.
* **Task 2.2:** Information retrieval:
* **Permissions:** Extract and format the mode (e.g., `drwxr-xr-x`).
* **Apparent vs. Actual Size:** Capture `st_size` (logical bytes) and `st_blocks` (actual disk allocation).


* **Task 2.3:** Symbolic Link Resolution: If an entry is a link (`S_ISLNK`), use `readlink()` to display its target path. Detect if the link is "broken" (target does not exist).

---

### Phase 3: Analytics Engine

**Goal:** Accumulate data during traversal to provide a summary report at the end of execution.

* **Task 3.1:** Aggregate total subtree size by summing `st_size`.
* **Task 3.2:** Sparse File Detection: Calculate the ratio of `(st_blocks * 512) / st_size`. Flag files where the physical size is significantly smaller than the logical size.
* **Task 3.3:** Final Report: Output the total number of directories, files, links, and the "Top 3" largest files found.

---

### Logical Milestones

1. **Hours 1–3:** Successful non-recursive listing of a single directory with file types.
2. **Hours 4–6:** Functional recursive tree with proper visual indentation.
3. **Hours 7–9:** Integration of `lstat` data and formatting of the permission/size strings.
4. **Hours 10–12:** Implementation of the summary analytics and final edge-case testing (e.g., permission denied folders).

---

### Key System Calls & Headers

* `<sys/stat.h>`: `lstat()`, macros like `S_ISDIR()`, `S_ISREG()`, `S_ISLNK()`.
* `<dirent.h>`: `struct dirent`, `opendir()`, `readdir()`.
* `<unistd.h>`: `readlink()`.
* `<stdio.h>`: `printf()`, `snprintf()` for path construction.