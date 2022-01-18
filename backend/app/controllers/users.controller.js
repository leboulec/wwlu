const db = require("../models");
const Users = db.users;
const Op = db.Sequelize.Op;

exports.findAll = (req, res) => {
    Users.findAll()
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
    if (!req.body.userid || !req.body.authorization) {
        res.status(400).send({
            message: "Content cannot be empty."
        });
        return;
    }

    const user = {
        userid: req.body.userid,
        authorization: req.body.authorization
    };

    Users.create(user)
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message: err.message || "Some error occured while creating user"
            });
        });
};