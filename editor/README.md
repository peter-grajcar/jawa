# Jawa Editor Support

## Vim

1. Copy `jawa.vim` to `~/.vim/syntax/`
2. Then add this line to your `.vimrc`
    ```vim
    au BufRead,BufNewFile *.jawa set filetype=jawa
    ```

## JetBrains IDE

### Linux

1. Copy `Jawa.xml` to `~/.config/JetBrains/<product><version>/filetypes`
2. Register file association in the IDE: **File | File Properties | Associate with File Type**

### MacOS

1. Copy `Jawa.xml` to `~/Library/Application Support/JetBrains/<product><version>/filetypes`
2. Register file association in the IDE: **File | File Properties | Associate with File Type**


### Windows

1. Copy `Jawa.xml` to `%APPDATA%\JetBrains\<product><version>/filetypes`
2. Register file association in the IDE: **File | File Properties | Associate with File Type**

