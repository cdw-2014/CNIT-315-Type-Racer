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
    "Life is what happens when you're busy making other plans.",
    "Spread love everywhere you go. Let no one ever come to you without leaving happier.",
    "Always remember that you are absolutely unique. Just like everyone else.",
    "Don't judge each day by the harvest you reap but by the seeds that you plant.",
    "The best and most beautiful things in the world cannot be seen or even touched - they must be felt with the heart.",
    "Do not go where the path may lead, go instead where there is no path and leave a trail.",
    "The real test is not whether you avoid this failure, because you won't. It's whether you let it harden or shame you into inaction, or whether you learn from it; whether you choose to persevere.",
    "Success is not final; failure is not fatal: It is the courage to continue that counts.",
    "There are no secrets to success. It is the result of preparation, hard work, and learning from failure.",
    "All the world’s a stage, and all the men and women merely players. They have their exits and their entrances; And one man in his time plays many parts.",
    "What we think, or what we know, or what we believe is, in the end, of little consequence. The only consequence is what we do.",
    "Try a thing you haven’t done three times. Once, to get over the fear of doing it. Twice, to learn how to do it. And a third time, to figure out whether you like it or not.",
    "When you go into court you are putting your fate into the hands of twelve people who weren’t smart enough to get out of jury duty.",
    "Just the fact that some geniuses were laughed at does not imply that all who are laughed at are geniuses. They laughed at Columbus, they laughed at Fulton, they laughed at the Wright brothers. But they also laughed at Bozo the Clown.",
    "We’ve heard that a million monkeys at a million keyboards could produce the complete works of Shakespeare; now, thanks to the Internet, we know that is not true.",
    "If there are no stupid questions, then what kind of questions do stupid people ask? Do they get smart just in time to ask questions?",
    "Imagination was given to man to compensate him for what he is not, and a sense of humor was provided to console him for what he is.",
    "The third-rate mind is only happy when it is thinking with the majority. The second-rate mind is only happy when it is thinking with the minority. The first-rate mind is only happy when it is thinking.",
    "History teaches us that men and nations behave wisely once they have exhausted all other alternatives.",
    "How many legs does a dog have, if you call the tail a leg? Four. Calling a tail a leg doesn’t make it a leg.",
    "You can fool some of the people all of the time, and all of the people some of the time, but you can not fool all of the people all of the time.",
    "Imagination is more important than knowledge. For knowledge is limited to all we now know and understand, while imagination embraces the entire world, and all there ever will be to know and understand.",
    "When you look at yourself from a universal standpoint, something inside always reminds or informs you that there are bigger and better things to worry about.",
    "When you sit with a nice girl for two hours, you think it’s only a minute. But when you sit on a hot stove for a minute, you think it’s two hours. That’s relativity.",
    "There are people who, instead of listening to what is being said to them, are already listening to what they are going to say themselves.",
    "Acquaintance, n.: A person whom we know well enough to borrow from, but not well enough to lend to.",
    "Perfection is achieved, not when there is nothing more to add, but when there is nothing left to take away.",
    "All truth passes through three stages. First, it is ridiculed. Second, it is violently opposed. Third, it is accepted as being self-evident.",
    "Trying to determine what is going on in the world by reading newspapers is like trying to tell the time by watching the second hand of a clock.",
    "The whole problem with the world is that fools and fanatics are always so certain of themselves, and wiser people so full of doubts.",
    "The most remarkable thing about my mother is that for thirty years she served the family nothing but leftovers. The original meal has never been found.",
    "The difference between a democracy and a dictatorship is that in a democracy you vote first and take orders later; in a dictatorship you don’t have to waste your time voting.",
    "Bureaucrats write memoranda both because they appear to be busy when they are writing and because the memos, once written, immediately become proof that they were busy.",
    "Never be afraid to laugh at yourself, after all, you could be missing out on the joke of the century.",
    "It is said that power corrupts, but actually it’s more true that power attracts the corruptible. The sane are usually attracted by other things than power.",
    "The direct use of force is such a poor solution to any problem, it is generally employed only by small children and large nations.",
    "The more you know, the more you realise how much you don’t know — the less you know, the more you think you know.",
    "Simple, clear purpose and principles give rise to complex and intelligent behavior. Complex rules and regulations give rise to simple and stupid behavior.",
    "Hofstadter’s Law: It always takes longer than you expect, even when you take into account Hofstadter’s Law.",
    "Be who you are and say what you feel, because those who mind don’t matter and those who matter don’t mind.",
    "I arise in the morning torn between a desire to improve (or save) the world and a desire to enjoy (or savor) the world. This makes it hard to plan the day.",
    "Just because your voice reaches halfway around the world doesn’t mean you are wiser than when it reached only to the end of the bar.",
    "There is only one way to happiness and that is to cease worrying about things which are beyond the power of our will.",
    "The opposite of the religious fanatic is not the fanatical atheist but the gentle cynic who cares not whether there is a god or not.",
    "A happy life is just a string of happy moments. But most people don’t allow the happy moment, because they’re so busy trying to get a happy life.",
    "Being in politics is like being a football coach. You have to be smart enough to understand the game, and dumb enough to think it’s important.",
    "America believes in education: the average professor earns more money in a year than a professional athlete earns in a whole week.",
    "We should be taught not to wait for inspiration to start a thing. Action always generates inspiration. Inspiration seldom generates action.",
    "A celebrity is a person who works hard all his life to become well known, then wears dark glasses to avoid being recognized.",
    "I do not feel obliged to believe that the same God who has endowed us with sense, reason, and intellect has intended us to forgo their use.",
    "Everyone is a genius at least once a year. The real geniuses simply have their bright ideas closer together.",
    "The fact that a believer is happier than a skeptic is no more to the point than the fact that a drunken man is happier than a sober one.",
    "You know you’re getting old when you stop to tie your shoelaces and wonder what else you could do while you’re down there.",
    "Are people more violently opposed to fur rather than leather because its much easier to harass rich women than motorcycle gangs?",
    "The nice part about being a pessimist is that you are constantly being either proven right or pleasantly surprised.",
    "There is a great difference between worry and concern. A worried person sees a problem, and a concerned person solves a problem.",
    "Science is facts; just as houses are made of stones, so is science made of facts; but a pile of stones is not a house and a collection of facts is not necessarily science.",
    "If you hate a person, you hate something in him that is part of yourself. What isn’t part of ourselves doesn’t disturb us.",
    "Part of the inhumanity of the computer is that, once it is competently programmed and working smoothly, it is completely honest.",
    "No matter what side of the argument you are on, you always find people on your side that you wish were on the other.",
    "Mistrust the man who finds everything good, the man who finds everything evil and still more the man who is indifferent to everything.",
    "What we think, or what we know, or what we believe is, in the end, of little consequence. The only consequence is what we do.",
    "Half the money I spend on advertising is wasted; the trouble is I don’t know which half.",
    "Before I got married I had six theories about bringing up children; now I have six children and no theories.",
    "Another flaw in the human character is that everybody wants to build and nobody wants to do maintenance.",
    "Besides the noble art of getting things done, there is the noble art of leaving things undone. The wisdom of life consists in the elimination of nonessentials.",
    "When hungry, eat your rice; when tired, close your eyes. Fools may laugh at me, but wise men will know what I mean.",
    "Most people are bothered by those passages of Scripture they do not understand, but the passages that bother me are those I do understand.",
    "It is by the goodness of God that in our country we have those three unspeakably precious things: freedom of speech, freedom of conscience, and the prudence never to practice either of them.",
    "A complex system that works is invariably found to have evolved from a simple system that works.",
    "The opposite of a correct statement is a false statement. But the opposite of a profound truth may well be another profound truth.",
    "Insane people are always sure that they are fine. It is only the sane people who are willing to admit that they are crazy.",
    "There is no greater impotence in all the world like knowing you are right and that the wave of the world is wrong, yet the wave crashes upon you.",
    "I have yet to see any problem, however complicated, which, when you looked at it in the right way, did not become still more complicated.",
    "Technology is dominated by two types of people: those who understand what they do not manage, and those who manage what they do not understand.",
    "A pessimist is one who makes difficulties of his opportunities; an optimist is one who makes opportunities of his difficulties.",
    "Grant me the serenity to accept the things I cannot change, courage to change thing I can, and wisdom to know the difference.",
    "Programming today is a race between software engineers striving to build bigger and better idiot-proof programs, and the Universe trying to produce bigger and better idiots. So far, the Universe is winning.",
    "It is a puzzling thing. The truth knocks on the door and you say, ‘Go away, I’m looking for the truth.’ and so it goes away. Puzzling.",
    "When one person suffers from a delusion It is called insanity. When many people suffer from a delusion It is called Religion.",
    "If the automobile had followed the same development cycle as the computer, a Rolls-Royce would today cost $100, get a million miles per gallon, and explode once a year, killing everyone inside."
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