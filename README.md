# CNIT-315-Type-Racer
A Type Racer clone written in ANSI C for CNIT 315 at Purdue University Fall 2020.

Team Members:
Alex Orzel
CJ Weaver
Connor Hetzler
Claire Rightly
Grace Chen

How to Run:
1. Open a terminal and go to the root directory of the program.
2. Compile with: `gcc type_racer.c -lcurl -o type_racer`
3. Run the program with ./type_racer
4. Follow the menu prompts.

External API:
For our distributed element of the project, we are pulling in passages for the type racer game from a REST API using the libcurl library. We could not find an existing quotes API that was reliable, so we created our own using Node.js. The server-side code for the API is in the quotes-api subfolder. The program will by default retrieve teh quotes from a production deployment of the API which we deployed to Heroku, but if you run into issues, you can also run the quotes API locally:
1. Install Nodejs and npm with: `sudo apt install nodejs` and `sudo apt install npm`
2. Open another terminal to run the server-side code, and navigate to the quotes-api subfolder.
3. Install the dependencies with: `npm install`
4. Run the server with: `npm start`
5. The API will be available on localhost:3000. The available endpoints include: /random and /random/:count where count is an integer value for the number of unique passages to retrieve.