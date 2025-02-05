# Turn-Based Chess

## Installation

### 1. **Downloading the Precompiled Version**
For a quicker setup, the precompiled version of the game is available for downloading, saved as `TurnBasedChessCompiled`. After downloading, it can simply be runned with the command:
   ```bash
   ./TurnBasedChessCompiled
  ```

### 2. **Compiling from Source**
The raw code can be downloaded and compiled following the next steps:

1. **Download the raw source code**:  
   The source code is available in the project files, saved as `TurnBasedChess.c`.

2. **Compile the code**:  
   The source code can be compiled by using any C compiler (e.g., GCC), by using the command:
   ```bash
   gcc -o TurnBasedChess TurnBasedChess.c```

## Usage

### Main menu.
The game has a main menu, with two submenus accessible through navigation from the main menu.

![imagen](https://github.com/user-attachments/assets/3e767d9b-5cf6-42bf-9217-30093bb7b439)


### Chess.
When you click the option "Play Chess" in the main menu, some display like this should appear:

![imagen](https://github.com/user-attachments/assets/cd47fd94-595f-48fb-bbee-933934a61c6e)

Here the user (black/white) needs to specify the piece that they want to be moved in the next format with the inputs separated with spaces:
1. Piece (P/Q/K...) 
2. Letter representative of the column (a, b, c...)
3. Number representative of the row (1, 2, 3...)

After the input has been completed (pressing the Enter key), another message should appear:

![imagen](https://github.com/user-attachments/assets/5479b0d3-799d-4a0b-8114-9615bb7f1002)

Here the user (black/white) needs to specify where they want to move the piece indicated in chess notation again separated by spaces:
1. Letter representative of the column (a, b, c...)
2. Number representative of the row (1, 2, 3...)

After the input has been completed (pressing the Enter key), the move should be finally performed and the match should begin.

![imagen](https://github.com/user-attachments/assets/d042c1cd-30ac-4673-ac01-29dbcdeafae9)

