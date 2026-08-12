<img src="https://github.com/marcotcal/PG-DBA-Tools/blob/master/GUI/icon.svg" width="128"> <img src="https://github.com/marcotcal/PG-DBA-Tools/blob/master/screen_shots/capture_001.png" width="228"> <img src="https://github.com/marcotcal/PG-DBA-Tools/blob/master/screen_shots/capture_002.png" width="228"> <img src="https://github.com/marcotcal/PG-DBA-Tools/blob/master/screen_shots/capture_003.png" width="228">

# A graphical tool for managing Postgresql servers

This tool is based on Melvin Davidson queries found on postgres-general mailing list.
I found it on https://github.com/JohnArchieMckown/PostgreSQL-DBA-tools (you can find the originals here), I made some modifications on the queries to adapt then to the newer versions and divided then on diferent folders by category. 
Some new queres were added to adapt the tool to the new versions of PostgreSQL

The intent is to help on DBA tasks for PostgreSQL

The choice of QT Framework was based on portability isues.
The PostgreSQL libpq will be used instead qpsql to avoid possible limitations. 
This will bring a little more work but also the benefits of total compatibility

There is now a subproject called **agent** 
This program is intend to be installed in the servers to supply additional information about the clusters
It will run as a service and will collect statistics.
The language in this subproject is **Go**. Why Go ? Because I want to learn Go :)

Any sugestion are wellcome!

## New  Implementations Comming

- The agent in Go Laguage.

- Update on PG DBA Tools GUI from QT Version 5 to QT Version 6.

- A complete front-end restructuring to make extensive use of the Agent.

### The Agent

TThe agent's objective will be to create a PostgreSQL and operating system metrics collector that is fully configurable by the DBA. The DBA will be able to create and modify metrics without altering the agent's code. 

The metrics will not be returned in the Prometheus model due to the required flexibility.

## **Notes**

About some false positives on QtCreator - Q_Object (incomplete type) - Please disable clang options on menu /help/about plugins
See configuration to choose the web engine

As each sql tool have a separated connection it is possible to open independent transactions. Also the sentings for query optimazations are applied only in the selected SQL Tool. 
