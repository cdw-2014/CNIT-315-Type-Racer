const http = require("http");
const express = require("express");
var monitor = require("uptime-robot");
const path = require("path");

const app = express();

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname + "/views/index.html"));
});

//const commandHandler = require("./commandHandler");
//commandHandler.handler();

let quotes = [
    "The greatest glory in living lies not in never falling, but in rising every time we fall.",
    "The way to get started is to quit talking and begin doing.",
    "Your time is limited, so don't waste it living someone else's life.",
    "If life were predictable it would cease to be life, and be without flavor.",
    "If you look at what you have in life, you'll always have more. If you look at what you don't have in life, you'll never have enough.",
    "If you set your goals ridiculously high and it's a failure, you will fail above everyone else's success.",
    "Life is what happens when you're busy making other plans."
];

app.get("/random", (req, res) => {
    const random = Math.floor(Math.random() * Math.floor(quotes.length));
    res.send(quotes[random]);
})

app.get("/random/:count", (req, res) => {
    let { count } = req.params;
    let selected = [];
    let indices = [];
    while (count > 0) {
        let random = Math.floor(Math.random() * Math.floor(quotes.length));
        if (!indices.includes(random)) {
            selected.push(quotes[random]);
            indices.push(random);
            count--;
        }
    }
    
    res.send(selected);
})

const listener = app.listen(3000, function() {
  console.log("Your app is listening on port " + listener.address().port);
});

