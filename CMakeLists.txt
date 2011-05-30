find_package (Compiz REQUIRED)

include (CompizPlugin)

compiz_plugin(bell PKGDEPS libcanberra)
