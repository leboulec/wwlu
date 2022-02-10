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
                message: err.message || "Some error occured while retrieving users."
            });
        });
};

exports.findOne = (req, res) => {
    const userid = req.query.userid;
    var condition = userid ? {userid: { [Op.like]: `%${userid}%`}} : null;
    Users.findAll({where: condition, raw: true}).then(data => {
        if (data.length === 0) {
            res.status(204).send(data);
        } else {
            res.status(200).send(data);
        }
    })
    .catch(err => {
        res.status(500).send({
            message: err.message || "Somme error occured while retrieving users."
        })
    })
}

exports.create = (req, res) => {
    console.log(req.body);
    if (!req.body.userid) {
        res.status(400).send({
            message: "Content cannot be empty."
        });
        return;
    }

    const user = {
        userid: req.body.userid,
        authorization: req.body.authorization
    };

    Users.findAll({where: {
        userid: req.body.userid
    }, raw: true}).then(exist => {
        if (exist.length === 0) {
            Users.create(user)
                .then(data => {
                    res.send(data);
                })
                .catch(err => {
                    res.status(500).send({
                        message: err.message || "Some error occured while creating user"
                    });
                });
        } else {
            res.status(404).send({
                message: "User already exists"
            });
        }
    });
};