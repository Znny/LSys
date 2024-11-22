Fun with Lindenmayer systems

Turtle interpretation of symbols:
  F : Move forward and draw a line
  f : Move forward without drawing a line
  + : Turn Right
  + - : Turn Left
  ^ : pitch up
  & : pitch down
  \ : roll left
  / : roll right
  | : turn around
  $ : rotate the turtle to vertical
  [ : start a branch
  ] : complete a branch
  { : start a polygon
  G : move forward and draw a line. do not record a vertex
  . : record a vertex in the current polygon
  } : complete a polygon
  ~ : incorporate a predefined surface
  ! : Decrement the diameter of segments
  ' : increment the current color index
  % : cut off the remainder of the branch

todo: 
    add rewriting rule loading
    add remaining l-system drawing symbols
    add view distance adjustment input
    add rotation input, rotation toggle


When a turtle 'draws' it should be in a rendering-agnostic way, 
and convert to a proper rendering format afterward
