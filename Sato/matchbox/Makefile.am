pngs = \
  close-button.png         \
  close-button-active.png  \
  arrow-down.png           \
  arrow-down-active.png    \
  titlebar-left-edge.png   \
  titlebar-right-edge.png  \
  titlebar-tile.png        \
  dia-left-edge.png        \
  dia-right-edge.png       \
  dia-tile.png
  
qvgapngs = \
  close-button-qvga.png         \
  close-button-active-qvga.png  \
  arrow-down-qvga.png           \
  arrow-down-active-qvga.png    \
  titlebar-left-edge-qvga.png   \
  titlebar-right-edge-qvga.png  \
  titlebar-tile-qvga.png        \
  dia-left-edge-qvga.png        \
  dia-right-edge-qvga.png       \
  dia-tile-qvga.png

themesdir = $(datadir)/themes/Sato/matchbox

if WITH_QVGA
themes_DATA = theme.xml $(qvgapngs)
EXTRA_DIST = theme.xml $(qvgapngs) 
else
themes_DATA = theme.xml $(pngs)
EXTRA_DIST = theme.xml $(pngs) 
endif

clean:
	rm -f theme.desktop
