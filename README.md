Linux shell implemented in C

To run:
    run "make".
    execute by using "./a.out".

Built in commands:
1.  cd [path]:
    It takes user to the directory specified in path.
    Path can be relative or absolute.
    "cd .." goes to previous directory.
    "cd ." goes to same directory.
    "cd" goes to ~.
    "cd -" goes to last directory.
2.  pwd
    Prints the present working directory.
3.  echo <string>
    Prints the string to terminal.
    Collapses all whitespaces to single space.
4.  ls -[al] <dir1> <file1> <dir2> <file2> .....
    Takes as input any number of files and           directories.
    For files, prints the file name.
    For directories, prints the contents in said directory.
    -a flag prints all hidden files for directories
    -l flag prints all contents of directory in long form and also gives total block size of the directory.
    -l for files prints the long form of file.
    "ls" is treated as "ls ."
    The order of flags and files can be arbitrary but the flags cannot be repeated.
5.  pinfo [pid]
    Atmost one argument can be provided.
    Prints the pid, process status, memory and executable path of process specified by given pid.
    If no pid is given ir prints the same for this shell program.
6.  history [number]
    If no number is given, prints the last 10 commands entered into the shell. If less than 10 commands have been given to shell, it prints all of them.
    If a number n is given, it prints the last n commands given to shell, If less than n commands have been given to shell, it prints all of them.
    Number has to be less than or equal to 20.
    It accepts only one argument.
7.  nightswatch -[n] <integer> <option>
    Takes as option either "interrupt" or "newborn"
    For interrupt, it displays the number of times any CPU has been interrupted by keyboard(IRQ 1). It displays every k seconds where n is inputted by user. This stops when the user presses "q+ENTER".
    For newborn, similar to interrupt, it prints the process id of the last process created. It prints it every k seconds when user inputs k. This stops when user inputs "q+ENTER".
8.  setenv var [value]
    Sets the value of the environment variable 'val' to 'value' if 'value' exists otherwise sets it to an empty string. It creates the environmental variable 'car' if it does not exist.
9.  unsetenv var
    Destroys the environment variable var.
10. jobs
    Lists all background jobs with their process status and pid.
11. kjob <job number> <signal number>
    Sends signal associated with <signal number> to the process with job number <job number> if process with <job number> exists.
12. fg <job number>
    Brings background job specified by <job number> to foreground and continues it.
13. bg <job number>
    Changes state of background job specified by <job number> from stopped to running.
14. overkill
    Kills all background processes.
15. quit
    Quits the shell. ctrl+d also quits the shell.

For other commands, the shell will fork the current process and create a child process and execute the command.
The shell also supports background processes(not for background processes). i.e. if the command ends with '&' with or without any space. The background process' job number and process id is printed when it begins and when it ends its name, process id and exit code is printed.
Signal handling for ctrl+c and ctrl+z is also implemented.
Exit status of previous command is represented by :') and :'( before prompt to represent success or failure respectively.
To quit, use ctrl+d or the command 'quit'.
REdirection and piping of commands has also been implemented in 1.c.
The home directory is '~'.

Explanation of files:
1. 1.c  :
    This writes the prompt and allocates user input to required function. It also redirects and pipes the input and output of files.
2. cd.c :
    cd() : Changes the directory
3. chint.c  :
    chint() : Checks if a given string is an integer or not.
4. echo.c   :
    echo() : Prints the string to terminal.
5. extract1.c   :
    extract_first() : Extracts the first word from the string.
6. funcpwd.c    :
    funcpwd() : Prints the present working directory.
7. history.c    :
    in_history() : Stores given command in history_file(upto 20), creates the file if it doesnot exist.
    history() : displays the history upto a specific number.
8. ls.c :
    print_file_permissions() : Prints the long form of a file or directory.
    ls() : Handles flags and separates formatting of files and directories.
9. nightswatch.c    :
    delay() : Time delay function which also checks for interrupts (q+ENTER).
    nightwatch() : Prints the required information every n seconds.
10. other.c :
    add_bg() : Adds background process to list of background processes.
    rem_bg() : Removes background process from list of background processes/
    find_bg : Finds the name of the background process associated with given pid.
    func() : Signal handler.
    other() : Handles the execution of both foreground and background processes stores background processes in required list.
11. pinfo.c :
    po10() : Computes powers of 10.
    pinfo() : Prints required information.
12. updir.c :
    update_dir() : Updates the prompt when directory is changed and changes the directory of execution of shell to home directory.
13. history_file : Stores the last 20 commands entered in the terminal.
14. fg.c :
    fg() : Brings a background process to foreground and takes back control of the terminal after the foreground process is terminated or stopped.
15. jobs.c :
    getstatus() : Returns the status of a given pid.
    jobs() : Lists all the jobs in the background.
16. kjob.c :
    kjob() : Sends specified signal to specified background job number.
17. overkill.c :
    overkill() : Sends a kill signal to all the processes in the background.
18. redirct.c :
    redirect() : Modifies the given string to account for redirection and returns flags for redirection.
19. setenv.c :
    sen() : Sets the value of an environment variable.
20. signal.c :
    ctrl() : Handles ctrl+z and ctrl+c signal.
21. unset.c :
    unsen() : Deletes the given environment variable.







