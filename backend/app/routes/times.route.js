module.exports = app => {
    const times = require("../controllers/times.controller");

    var router = require("express").Router();

    // Find all alerts
    router.get("/", times.findAll);
    router.post("/", times.create);

    app.use("/api/times", router);
}
