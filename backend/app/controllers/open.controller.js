const db = require("../models");
const Open = db.open;
const Op = db.Sequelize.Op;

exports.update = (req, res) => {
    Open.update({open: "1"}, {
        where: {id: 1}
    })
        .then(num => {
            console.log(num);
            if (num == 1) {
                res.send("Changed state to open");
            } else {
                res.send("State is already open");
            }
        })
        .catch(err => {
            res.status(500).send({
                message: "Error while updating check ${id}"
            });
        })
};

exports.findOne = (req, res) => {
    Open.findByPk(1, {raw: true})
        .then(data => {
            console.log(data.open);
            if (data.open == 0) {
                res.status(300).send(data);
            } else {
                res.status(301).send(data);
            }
            Open.update({open: "0"}, {where : {id: 1}}).then(tmp => console.log(tmp));
        })
        .catch(err => {
            res.status(500).send({
                message: "Error retrieving open state"
            });
        });
};