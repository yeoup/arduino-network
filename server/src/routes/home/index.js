"use strict";

const router = require("express").Router();

router.get("/test", (req, res) => {
    res.send(req.body);
})

module.exports = router;