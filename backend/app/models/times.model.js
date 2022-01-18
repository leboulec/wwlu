module.exports = (sequelize, Sequelize) => {
    const Times = sequelize.define(
        "times",
        {
            id: {
                type: Sequelize.INTEGER,
                primaryKey: true,
                autoIncrement: true
            },
            timeOpen: {
                type: Sequelize.DATE
            },
            openedBy: {
                type: Sequelize.INTEGER,
            }
        }, {
            timestamps: false,
        }
    )
    return Times;
}