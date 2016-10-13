<h1 align=center>SwBasicTetris</h1>
<p align=center>This is simple tetris.</p>


<br><br>

## Usage
Please check your options.
```c++
void init(){

    SPEED = 1000; // Tetris game speed (ms)
    GHOST_MODE = true // Shows a drop position
    DETECT_CHECK = true; // Check colision
    ROTATE_CORNER = true; // Rotate block by corner
    SHOW_NEXT_BLOCK = true; // Show next block
    ENABLE_CLEAR_BLOCK = false; // Create CLEAR_BLOCK (Remove two line from bottom)
    ENABLE_BOMB_BLOCK = false; // Create BOMB_BLOCK (Remove blocks of the surrounding)
    
}
```

How to start a game.
```c++
int main(){
    GameStart(); // Game start with this function.
}
```


<br><br>
## License

    Copyright 2016 Taewoo You

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
