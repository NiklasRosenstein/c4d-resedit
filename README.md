# ResEdit for Cinema 4D

This fork aims to eliminate curcial bugs in the original ResEdit plugin.

- [Changelog][Milestones]
- [Downloads][Releases]

# Build Instructions

1. Get [Craftr][] and [Ninja][]
2. Copy the [maxon.c4d][] Craftr module into the plugins directory
3. Check out the code from this repository
4. `mkdir build && chdir build`
5. `craftr -c.. export && ninja`

  [Releases]: https://github.com/nr-plugins/resedit/releases
  [Milestones]: https://github.com/nr-plugins/resedit/milestones?state=closed
  [Craftr]: https://github.com/craftr-build/craftr
  [Ninja]: https://github.com/martine/ninja
  [maxon.c4d]: https://github.com/craftr-build/maxon.c4d

----

Copyright (C) 2015 MAXON Computer GmbH  
Fork by Niklas Rosenstein  http://niklasrosenstein.com  
Source Code at  https://github.com/nr-plugins/resedit
