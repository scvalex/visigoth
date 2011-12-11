Visigoth
========

> Qt4 Graph Visualizations

Development
-----------

Development is driven by the `Makefile`:

To build the project, run:

    make build

To run the tests, use:

    make test

The above also generates coverage statistics, you can view these with:

    make local-cover

(Note that the [Cobertura](http://cobertura.sourceforge.net/) plugin
the CI server uses relies on `make ci-cover`)

To run the app, use:

    make run

Twitter plugin
--------------

The Twitter plugin uses the [QOAuth](https://github.com/ayoy/qoauth)
and [QCA](http://delta.affinix.com/qca/) libraries.  If these are not
found when building, the plugin will not be enabled.
