# Simple Menu
Simple launcher for the new bittboy.

Controls:
- TB: previous section
- TA: next section
- A: launch game
- B: switch between menu and artwork modes
- Left or Right: next or previous 10 games
- Select: 
  - +Start: shutdown
  - +Up: overclocking, underclocking, normal (cyclic)
  - +Left or Right: scroll to the next next letter, forward or back
- R: go into or exit the favorites screen
- Start, outside favorites: adds a game to favorites
- Start, in favorites: removes the game from favorites

Notes:
- Sections need to be added manually in the config file, the numbers at the end are the colors of the header, footer, body and associated text, in rgb form.
- Don't delete or modify the Scripts or Favorites sections, that will definitely break it.
- At this point, no command line parameters are supported.
- Favorites are only written if they were changed and, even then, only when launching a game or quitting the menu, to minimize writes to the sd card.
