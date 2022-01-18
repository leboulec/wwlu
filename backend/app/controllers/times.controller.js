const db = require("../models");
const Times = db.times;
const Op = db.Sequelize.Op;

exports.findAll = (req, res) => {
    Times.findAll()
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message: err.message || "Some error occured while retrieving alerts."
            });
        });
};

exports.create = (req, res) => {
    console.log(req.body);
    if (!req.body.timeOpened || !req.body.openedBy) {
        res.status(400).send({
            message: "Content cannot be empty."
        });
        return;
    }

    const time = {
        timeOpen: Date(req.body.timeOpen),
        openedBy: req.body.openedBy
    };

    Times.create(time)
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message: err.message || "Some error occured while creating time"
            });
        });
};