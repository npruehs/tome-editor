[![AppVeyor](https://img.shields.io/appveyor/ci/npruehs/tome-editor.svg?maxAge=2592000)](https://ci.appveyor.com/project/npruehs/tome-editor)
[![GitHub tag](https://img.shields.io/github/tag/npruehs/tome-editor.svg?maxAge=2592000)](https://github.com/npruehs/tome-editor/releases)
[![license](https://img.shields.io/github/license/npruehs/tome-editor.svg?maxAge=2592000)](https://github.com/npruehs/tome-editor/blob/develop/LICENSE)

# Tome

Tome is a generic data editor for games supporting arbitrary input and output formats.

Developing games is all about data. With game systems for modifying data and user interfaces for presenting it, finding a sophisticated data model is key to creating successful games. Even more, being able to change this data and iterate on a day-to-day basis is mandatory in today's production cycles.

Tome solves two problems that arise with every new game you build: First, you can import the definition of your game's data so game designers can create new data records and modify existing ones very conveniently. Second, you can export the results to any format your game engine understands.

The tool is pretty self-explanatory. However, to give you a headstart, feel free to take a look at the [Tome wiki pages](https://github.com/npruehs/game-data-editor/wiki).

## Features

* Creating data field definitions including constraints
* Editing record values conveniently with tooltips and validation
* Creating custom data types such as enumerations, lists and maps
* Combining fields into components
* Hierarchical record structure with inherited fields
* Arbitrary output data formats through templates
* Splitting data up into multiple files for improved collaboration
* Run integrity checks to verify all your data
* Advanced search features, such as finding all references to a specific record
* Command line support for exporting data without showing a GUI

## Development Cycle

Feel free to take a look at the public [development roadmap](https://github.com/npruehs/tome-editor/milestones). 

We know that using a tool like Tome in production requires you to be completely sure about stability and compatibility. Thus, new releases of Tome are created using [Semantic Versioning](http://semver.org/). In short:

* Version numbers are specified as MAJOR.MINOR.PATCH.
* MAJOR version increases indicate incompatible changes with respect to Tome [data types and/or files](https://github.com/npruehs/game-data-editor/wiki/Project-Structure).
* MINOR version increases indicate new functionality that are backwards-compatible.
* PATCH version increases indicate backwards-compatible bug fixes.

## Bugs & Feature Requests

We are sorry that you've experienced issues or are missing a feature! After verifying that you are using the [latest version](https://github.com/npruehs/game-data-editor/releases) of Tome and having checked whether a [similar issue](https://github.com/npruehs/game-data-editor/issues) has already been reported, feel free to [open a new issue](https://github.com/npruehs/game-data-editor/issues/new). In order to help us resolving your problem as fast as possible, please include the following details in your report:

* Steps to reproduce
* What happened?
* What did you expect to happen?

After being able to reproduce the issue, we'll look into fixing it immediately.

## Contributing

You'd like to help make Tome even more awesome? Seems like today's our lucky day! In order to maintain stability of the tool and its code base, please adhere to the following steps, and we'll be pleased to include your additions in our next release.

Note that Tome is distributed under the [GNU Lesser General Public License 3.0](https://github.com/npruehs/game-data-editor/blob/master/LICENSE). So will be your code.

### Step 1: Choose what to do

If you've got no idea how to help, head over to our [issue tracker](https://github.com/npruehs/game-data-editor/issues) and see what you'd like to do most. You can basically pick anything you want to, as long as it's not already assigned to anyone.

If you know exactly what you're missing, [open a new issue](https://github.com/npruehs/game-data-editor/issues/new) to begin a short discussion about your idea and how it fits the project. If we all agree, you're good to go!

### Step 2: Fork the project and check out the code

Tome is developed using the [GitFlow branching model](http://nvie.com/posts/a-successful-git-branching-model/). In order to contribute, you should check out the latest develop branch, and create a new feature or hotfix branch to be merged back.

### Step 3: Implement your feature or bugfix

Tome is based on [Qt 5.7](http://www.qt.io/).

You might also take a look at our [development wiki](https://github.com/npruehs/game-data-editor/wiki) in order to get a better understanding of how everything's tied together.

### Step 4: Open a pull request

Finally, [open a pull request](https://help.github.com/articles/creating-a-pull-request/) so we can review your changes together, and finally integrate it into the next release.

## License

Tome is released under the [GNU Lesser General Public License 3.0](https://github.com/npruehs/game-data-editor/blob/master/LICENSE).
