# Simple Menu
Simple launcher for the new bittboy.

Controls:
- TB: previous section
- TA: next section
- A: launch game
- B: switch between menu and artwork modes
- Select: 
  - +Start: shutdown
  - +Up: overclocking, underclocking, normal (cyclic)
  - +Down: enable or disable alphabetical paging for the current section
  - +Left or Right: if in alphabetical paging mode, scroll to the next page (instead of to the next letter), forward or back, which is the default behavior for left and right when alphabetical paging is disabled.
- R: go into or exit the favorites screen
- Start outside favorites: adds a game to favorites
- Start in favorites: removes the game from favorites

Notes:
- Sections need to be added manually in the config file, the numbers at the end are the colors of the header, footer, body and associated text, in rgb form.
- Don't delete or modify the Scripts or Favorites sections, that will definitely break it.
- At this point, no command line parameters are supported.
- Favorites are only written if they were changed and, even then, only when launching a game or quitting the menu, to minimize writes to the sd card.
