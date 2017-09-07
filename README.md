# Plain text to Database - P2D
A library to put plain text data on database.

# Aim Values

* Simple use.
* Fast implementation to this library utilizer.
* Fast execution.

# Features

## Functional

- [X] Read text file on ASCII encoding.
- [ ] Read text file on any other encoding.
- [X] Accept file descripted by orderly field name and length.
- [X] Accept file descripted by field name, position and length.
- [X] Put data on MongoDB.
- [ ] Put data on SQL databases.
- [ ] Put data on memory structures.
- [ ] Direct integration with table discover software (like Tabula) to get layout of file.
- [ ] Discover desired layout of file using AI and sugest.
- [ ] Describe a layout with repeated subregions.
- [ ] Describe a layout starting with. (like end with delimitator).
- [ ] Process subregions to diferent database entities (collections, tables, ...)

## Performace and Availability

- [ ] Optimized performace to pure only positional files.
- [ ] Discover optimal processing model to current computing architecture (thread support, amount of RAM, processor speed, memory bus speed, storage bus speed).
- [ ] Process many parts of file in parallel.
- [ ] Read file from disk at optimal speed (memory mapping, streaming, optimal block reading, amount of storage units, etc).
- [ ] Orchestrate cluster of application to use many isolated machines infrastructure. (docker, kubernetes, cluster middleware, etc.)
- [ ] Use optimal processing model.

# Requirements

* C++ 14
* Mongo DB driver. (mongo-cxx)
* STL
* GNU Make

# Code style

This project use the Google C++ style guide

