# RoboNLP

NOTE: This file is available in Italian [here](README-IT.md)!

**RoboNLP** is a project developed during the Operating Systems Laboratory course at the Bachelor's Degree in Computer Science at the University of Naples Federico II.

The project's goal is to implement a server written in C that, using system calls, instructs a Python client to manage the **Furhat robot**.

## Architecture

The application is divided into two main parts:

* a **server**, written in C, uses system calls for thread management, sockets, and file handling, and utilizes two external libraries ([picohttpparser](https://github.com/h2o/picohttpparser) and [cJSON](https://github.com/DaveGamble/cJSON)) for parsing complex data structures;
* a **client**, written in Python, uses furhat-remote-api for robot interaction and [openai](https://github.com/openai/openai-python) for natural language interpretation and articulation.

## Description

At startup, the robot will ask the user to identify themselves. If the user is not registered, they will be given a questionnaire in the form of [TIPI](https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/), and the results will be saved on the server. Every particular detail about the user will be saved on the server so that the robot can remember it in the next session.

Once the registration phase is complete, the robot will enter chatting mode, interacting with the user while trying to emulate their personality.

## Installation

> **NOTE**: the following steps have been tested on MacOS but should also work on Linux and Windows via WLS.

First, execute the server setup file. Moving to the **server** folder from the terminal, run the following command:

```sh
./setup.sh
```

This will install the necessary dependencies in the **server/libs** folder.

Then create a **.env** file in the project root and insert your API Token inside it.

### Client development environment setup (optional)

Opening the repository in VSCode, and installing the appropriate extensions for Python development, you can look for the **client/requirements.txt** file and click the "Initialize virtual environment" button at the bottom right. After this operation, VSCode will recognize third-party libraries and activate autocompletion.

## Startup

Before starting the application, it's necessary to run the remote-api skill on the Furhat robot or its simulator. In version 2.8.0 of the simulator, this skill is already present in the Furhat SDK application installation available at **~/.furhat/launcher/Plugins/furhat-remote-api.skill**

To start the application, simply run the following command in the project root:

```sh
docker compose up
```
