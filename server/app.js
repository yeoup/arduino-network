// Modules
const express = require("express");
const app = express();
const bodyParser = require("body-parser");
const dotenv = require("dotenv");
dotenv.config();

// Routers
const home = require("./src");

// Middleware
app.use(bodyParser.json({ extended: true })); // body-parser
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.raw({ extended: true }));
app.use(express.static("images")); // static folder

app.use("/", home);

module.exports = app;