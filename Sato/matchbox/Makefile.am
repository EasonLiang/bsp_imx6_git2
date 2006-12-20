pngs = \
  close-button.png         \
  titlebar-left-edge.png   \
  titlebar-right-edge.png  \
  titlebar-tile.png        \
  dia-left-edge.png        \
  dia-right-edge.png       \
  dia-tile.png

themesdir = $(datadir)/themes/Sato/matchbox
themes_DATA = theme.xml $(pngs) theme.desktop

EXTRA_DIST = theme.xml $(pngs) theme.desktop.in

theme.desktop: theme.desktop.in
	sed -e "s,PREFIX,@prefix@," theme.desktop.in > theme.desktop

clean:
	rm -f theme.desktop
