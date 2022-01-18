const db = require("../models");
const Open = db.open;
const Op = db.Sequelize.Op;

exports.update = (req, res) => {
    Open.update(req.body, {
        where: {id: 0}
    })
        .then(num => {
            if (num == 1) {
                res.send({
                    message: "Check updated successfully."
                });
            } else {
                res.send({
                    message: "Cannot update open state"
                });
            }
        })
        .catch(err => {
            res.status(500).send({
                message: "Error while updating check ${id}"
            });
        })
};

exports.findOne = (req, res) => {
    Open.findByPk(0)
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message: "Error retrieving open state"
            });
        });
};