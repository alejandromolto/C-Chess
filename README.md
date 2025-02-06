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
   gcc -o TurnBasedChess TurnBasedChess.c

## Usage

### Main menu.
The game has a main menu, with two submenus accessible through navigation from the main menu.

![Image1](https://github.com/user-attachments/assets/903a6a4c-083c-4654-99f7-6abd0007762f)


### Chess.
When you click the option "Play Chess" in the main menu, some display like this should appear:

![Image2](https://github.com/user-attachments/assets/c765901c-ceb0-42d7-b5ff-54b9f4ea94bc)

Here the user (black/white) needs to specify the piece that they want to be moved in the next format with the inputs separated with spaces:
1. Piece (P/Q/K...) 
2. Letter representative of the column (a, b, c...)
3. Number representative of the row (1, 2, 3...)

After the input has been completed (pressing the Enter key), another message should appear:

![Image3](https://github.com/user-attachments/assets/3ab69e7c-8d8f-4d1d-906b-1f35bb75809a)

Here the user (black/white) needs to specify where they want to move the piece indicated in chess notation again separated by spaces:
1. Letter representative of the column (a, b, c...)
2. Number representative of the row (1, 2, 3...)

After the input has been completed (pressing the Enter key), the move should be finally performed and the match should begin.

![image4](https://github.com/user-attachments/assets/2d159cc8-19e2-4250-89ea-74836bc52da4)

