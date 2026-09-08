<h3>Rubik's Cube Solution Using Iterative Deepening Depth First Search (IDDFS)</h3>
<p>
This program is written in C++17 and uses the Eclipse C++ IDE 2026-06 with GCC compiler and MINGW linker. 
The solution in Rubik's cube is to have all the cube faces the same color.  The colors are White (Up face),
Yellow (Down face), Green (Left face), Blue (Right face), Red (Front face), and Orange (Back face).  The faces
are denoted as: <b>U</b>(Up), <b>D</b>(Down), <b>L</b>(Left), <b>R</b>(Right), <b>F</b>(Front), and <b>B</b>(Back).  
<b>The 18 valid Rubik's cube moves are:</b>
<ol>
  <li>R (Right face CW 90 deg)</li>
  <li>R' (Right face CCW 90 deg) </li>
  <li>R2 (Right face 180 deg)</li>
  <li>L (Left face CW 90 deg)</li>
  <li>L' (Left face CCW 90 deg)</li>
  <li>L2 (Left face 180 deg)</li>
  <li>U (Up face CW 90 deg)</li>
  <li>U' (Up face CCW 90 deg)</li>
  <li>U2 (Up face 180 deg)</li>
  <li>D (Down face CW 90 deg)</li>
  <li>D' (Down face CCW 90 deg)</li>
  <li>D2 (Down face 180 deg)</li>
  <li>F (Front face CW 90 deg)</li>
  <li>F' (Front face CCw 90 deg)</li>
  <li>F2 (Front face 180 deg)</li>
  <li>B (Back face CW 90 deg)</li>
  <li>B' (Back face CCW 90 deg)</li>
  <li>B2 (Back face 180 deg)</li>
</ol>
This program supports up to ten moves and more than one trial can be run for the requested move number.
The algorithm will find the shortest manhattan distance; that is, the fewest number of moves to find
the solution, which is all the faces having the same color.  The algorithm uses a bounded depth first
search (bounded DFS).  First DFS is run with depth one.  If the solution is not found, then DFS is run
with depth two.  This continues with DFS being run at increasingly greater depths. On my laptop, an
eight unique move sequence could take about three hours.  The solution time increases by 18 for every
additional move.  So you can expect solution times of 3*18 hours for a nine-move unique sequence and 
3*18*18 for a ten-move unique sequence.  By a unique sequence, I mean that there are no moves that reverse
the previous ones or repetitive moves that wind up at the same place such as R R' or L2 L2.
</p>

<p>
The user can enter the moves on the command line or allow the program to randomly choose the moves.  If
you want to specify the moves then enter the twists after the program name with a space separating the moves.
The scrambled cube faces are displayed along with the move sequence.  As the IDDFS proceeds, the bound is displayed
for the current DFS.  When the solution is found, the solution moves are displayed along with the cube faces for 
the solution; the faces should all have the same color.  A Python script can display a 3D scatterplot of the scrambled
and solved Rubik's cube.  You can select the F-R-U or B-L-D faces to be displayed.  The input arguments are [0,1] [0,1]:
[(scrambled|solved) (F-R-U | B-L-D)]
</p>

<h4>7 moves, specify moves from command line, 1053 seconds</h4>
<img width="635" height="550" alt="image" src="https://github.com/user-attachments/assets/332878d1-2d20-46cc-8db0-30716f855789" />
<img width="635" height="549" alt="image" src="https://github.com/user-attachments/assets/dabc0cc6-7abe-446e-9622-7bef40c9dcec" />
<img width="941" height="678" alt="image" src="https://github.com/user-attachments/assets/67ed11a0-83d2-4631-bd16-d680bb7bb752" />
<img width="634" height="549" alt="image" src="https://github.com/user-attachments/assets/70a54049-cbab-4af2-8c69-6aff7e40d2a8" />
<img width="637" height="549" alt="image" src="https://github.com/user-attachments/assets/34f2bbb0-f0a8-4689-a094-e9ac53088ee8" />
<h4>8 moves, random moves, 8731 seconds</h4>
<img width="637" height="553" alt="image" src="https://github.com/user-attachments/assets/f2bc3dd9-1fe4-47ae-adb3-208a8a30972d" />
<img width="1008" height="739" alt="image" src="https://github.com/user-attachments/assets/1c500384-7a82-495d-bc5e-6577c2646328" />
<img width="634" height="550" alt="image" src="https://github.com/user-attachments/assets/797ff7cb-11d9-456f-86a8-10b2b5c49f0a" />

