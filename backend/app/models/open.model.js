module.exports = (sequelize, Sequelize) => {
    const Open = sequelize.define(
        "open",
        {
            id: {
                type: Sequelize.INTEGER,
                primaryKey: true,
                autoIncrement: true
            },
            open: {
                type: Sequelize.INTEGER
            }
        }, {
            timestamps: false,
        }
    )
    return Open;
}
