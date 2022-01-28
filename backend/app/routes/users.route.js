module.exports = app => {
    const users = require("../controllers/users.controller");

    var router = require("express").Router();

    // Find all alerts
    router.get("/all", users.findAll);
    router.post("/", users.create);
    router.get("/", users.findOne)

    app.use("/api/users", router);
}
