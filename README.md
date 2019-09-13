# simplemenu
Simple launcher for the new bittboy.

Controls:
- B: previous section
- A: launch game
- TA: next section
- TB: overclocking, underclocking, normal (cyclic)
- R: go into or exit the favorites screen
- Start outside favorites: adds a game to favorites
- Start in favorites: removes the game from favorites
- Select: switch between menu and artwork modes
- Start + Select: shutdown

Notes:
- Sections need to be added manually in the config file, the numbers at the end are the colors of the header, footer, body and associated text, in rgb form.
- At this point, no command line parameters are supported.
- Favorites are only written if they were changed and, even then, only when launching a game or quitting the menu, to minimize writes to the sd card.
