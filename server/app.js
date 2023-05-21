"use strict";

// Modules
const express = require("express");
const app = express();
const dotenv = require("dotenv");
dotenv.config();

// Routers
const home = require("./src/routes/home");

// Middleware
app.use(express.json()) // body-parser
app.use(express.urlencoded({ extended: true }))

app.use("/", home);

module.exports = app;