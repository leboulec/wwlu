module.exports = app => {
    var router = require("express").Router();

    router.get("/", (req, res) => {
        res.json({message: "Hello World!"});
    });

    app.use(router);
}