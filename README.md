# Simple Menu
Simple launcher for the new bittboy.

Controls:
- TB: switch between menu and screenshot modes.
- TA: launch game.
- B, in favorites: removes the game from favorites.
- B, outside of favorites: adds a game to favorites.
- Left: previous 10 games.
- Right: next 10 games.
- Up: previous game.
- Down: next game.
- A: hotkey 
  - +Left: previous section.
  - +Right: next section.
  - +Up: previous letter.
  - +Down: next letter.
- R: go into or exit the favorites screen.
- Start: cycle through overclocking (798), underclocking(408) and normal(702).
- Select+Start: shut down.

Notes:
- Screen captures should be inside the rom folder, have the same name as their companion rom and the "png" extension.
- Sections need to be added manually in the config file, the numbers at the end are the colors of the header background, header text, body background, body text, highlighted text background and highlighted text foreground, expressed in RGB form.
- Don't delete or modify the Scripts or Favorites (9999 in 1) sections; that will definitely break it.
- At this point, no command line parameters are supported.
- Favorites are only written if they were changed and, even then, only when launching a game or quitting the menu, to minimize writes to the sd card.
