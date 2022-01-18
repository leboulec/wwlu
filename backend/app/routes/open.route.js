module.exports = app => {
    const open = require("../controllers/open.controller");

    var router = require("express").Router();

    // Find all alerts
    router.get("/", open.findOne);
    router.post("/", open.update);

    app.use("/api/times", router);
}

