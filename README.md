# RoboNLP

NOTE: This guide file is available in Italian [here](README-IT.md)!

**RoboNLP** is a project developed during the Operating Systems Laboratory course at the Bachelor's Degree in Computer Science at the University of Naples Federico II

The goal of the project is to implement a server written in C that, by using system calls, instructs a client written in Python to manage the **robot Furhat**.

## Architecture

The application is divided into two main parts:

* a **server**, written in C, it employs system calls to handle threads, sockets and file management and makes use of two external libraries ([picohttpparser](https://github.com/h2o/picohttpparser) and [cJSON](https://github.com/DaveGamble/cJSON)) to parse complex data structures;
* a **client**, written in Python, that uses furhat-remote-api to interact with the robot and [openai](https://github.com/openai/openai-python) to interpret and articulate natural language.

## Description

At launch, the robot will ask the user to identify themselves. If the user is not registeres, they will be given a questionnaire in the form [TIPI] (https://gosling.psy.utexas.edu/scales-weve-developed/ten-item-personality-measure-tipi/) and the results will be saved in the server. Every particular detail of the user will be saved in the server so that the robot may preserve memory of it in the next session.

Once the registration phase is complete, the robot will enter its chat mode, and will interact with the user while trying to emulate their personality.


## Installation

> **NOTE**: the following steps have been tested on a MacOS system, but should also work on Linux and Windows through WLS.

First you need to run the server setup file. After moving into the **server** folder trhough the terminal, execute the following command:

```sh
./setup.sh
```

This will install all necessary dependencies in the folder **server/libs**.

### Client development environment installation (optional)

By opening the repository on VSCode and installing all necessary extensions for Python development, you will be able to look for the **client/requirements.txt** file and click on the button at the bottom right corner "Initialize virtual environment". By the end of this operations, VSCode will recognize all third party libraries and will activate autocomplete.

## Launch

Before starting the application, it's necessary to execute the remote-api skill on the Furhat robot or its simulator. In the version 2.8.0 of the simulator, the skill is already present in the installation of the Furhat SDK available at **~/.furhat/launcher/Plugins/furhat-remote-api.skill**

To launch the application simplu execute the following command at the root folder of the project:

```sh
docker compose up
```

## Configuration

> **TODO: Scrivere istruzioni di configurazione**
