# Systems Programming - 1st Assignment
**Nikolaos Prevolis - sdi2000172**

## 🛠 Compilation & Execution
In the main directory:
* **To compile:** `make`
* **Troubleshooting:** If you see the message `make: Nothing to be done for 'all'`, you must run `make clean` first.
* **To run:** `./mysh`

## 📁 File Organization
* **`source/`**: All C source code files.
* **`headers/`**: All header files.
* **`bin/`**: Object files (`.o`) generated during compilation.
* **Root Directory**: Contains the `Makefile`, `README`, completion report, the final executable, and any files created by the program (e.g., output files).

---

## ⌨️ Execution Instructions
1.  **Alias Arguments:** Calling an alias with additional arguments is **not** supported. Any necessary arguments must be included at the time the alias is created.
2.  **Alias Commands:** To create an alias: `createAlias {aliasname} {command}`. To delete: `deleteAlias {aliasname}`. Commands should be entered without quotes.
3.  **History:** Use the `myHistory` command. The command selection occurs after the history is printed.
4.  **Special Characters:** Aside from wildcards, special characters (`<`, `>`, `>>`, `&`, `|`) **must** be separated by spaces from the preceding and following words (e.g., `command < input > output`).
5.  **Semicolons:** The `;` character does not need to be separated by a space from the preceding word.

---

## 📝 Component Descriptions

### 1. `main.c`
* **Initialization:** Creates the structures that hold aliases and history.
* **Main Loop:** Runs an infinite loop using `getline()` to read from `stdin` and pass the input to `tokenise()`.
* **Logic:** Input is first split by `;` so commands like `command1; command2;` run sequentially.
* **`strtok_r()` vs `strtok()`:** Because the program calls `strtok` in multiple places to split sets of strings, `strtok_r()` is used instead. This allows specific memory points to be passed in each call, preventing a call in `tokenise()` from interfering with the input processing in `main()`.
* **Visuals:** `prompt()` uses escape characters to change the color of the shell prompt.
* **History Entry:** If a command runs successfully, `tokenise` returns 0 and it is added to history. To achieve this, the input is copied before being split by `strtok_r()`, as the original string is modified during tokenization.

### 2. `tokenise.c`
* **`tokenise()`:** Responsible for splitting commands into words. It copies the command twice: once for the secondary splitting loop and once to pass the intact input to `pipe()`.
    * It calculates `argc` to allocate memory for the `argv` vector.
    * As it populates `argv`, it checks for special characters (redirection, background, pipe, wildcards) and updates the relevant flags and indices.
    * `argv` is NULL-terminated for compatibility with `execvp()`.
    * It checks for internal commands (`createalias`, `destroyalias`, `myHistory`).
    * If `myHistory` is called, `print_history()` returns the selected command; `tokenise()` then calls itself recursively with that command.
    * Checks if the first word matches a saved alias.
* **Wildcards:** If wildcards are found, `glob` is used to access an array of filenames and their count. If none are found, the count is set to 1 to run the loop once.
* **Execution:** For each file/command, it routes to `redirect()`, `pipe_exec()`, or `exec()`.
* **`exec()`:** Calls `fork()`. The child calls `execvp()`, while the parent waits (unless it is a background process).

### 3. `redirect.c`
* **`which_red()`:** Checks if a word is a redirection character.
    * Flag logic: 1 = Output only, 2 = Input only, 3 = Both.
    * The `<<` character is treated the same as `<` for logic purposes.
* **`redirect()`:** Calls `fork()`. Based on the flag, it opens the necessary files and swaps the streams using `dup2()`.
    * For output with `>>`, it uses the `O_APPEND` flag.
    * Indices are used to set the redirection character and the filename to `NULL` in `argv` so they are ignored by `execvp()`.
    * Parent waits (if not background) and then resets the `stdin`/`stdout` streams.

### 4. `pipe.c`
* **`pipe_exec()`:** Finds the index of the `|` character and splits the command into two parts.
    * Creates a pipe and performs two forks (one for each command part).
    * **Child 1:** Redirects output to the pipe and calls `tokenise()` for the first part.
    * **Child 2:** Redirects input from the pipe and calls `tokenise()` for the second part.
    * Calls `tokenise()` instead of `execvp()` to handle potential special characters within the piped segments.

### 5. `wild.c` & `background.c`
* **`wild_check()`**: Validates if a word is a wildcard character.
* **`background_check()`**: Validates if a word is a background execution character (`&`).

### 6. `history.c` / `history.h`
* **Structure:** `myHistory` contains an array for `MAX_HISTORY` commands and a counter.
* **`add_history()`:** Adds successful commands. If full, it shifts all entries to the left (FIFO), freeing the oldest to make room for the newest.
* **`print_history()`:** Prints commands in reverse order. It uses `getline()` for user input. If Enter is pressed, it returns `NULL`. If an invalid number is given, it stays in the loop so the user can try again without re-typing the command.

### 7. `alias.c` / `alias.h`
* **Structure:** Stores the alias name and the corresponding command string.
* **`create_alias()`:** Allocates memory for a new alias. It does **not** check if the name conflicts with existing system commands.
* **`check_alias()`:** Checks if the first word of input is a saved alias and returns the command or `NULL`.
* **`destroy_alias()`:** Finds the alias index. If it is the last one, it is freed; otherwise, it is replaced by the last alias in the list to maintain the array.
* **`free_alias()`:** Frees all allocated memory for aliases.
