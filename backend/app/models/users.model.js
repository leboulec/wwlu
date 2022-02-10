module.exports = (sequelize, Sequelize) => {
    const Users = sequelize.define(
        "users",
        {
            id: {
                type: Sequelize.INTEGER,
                primaryKey: true,
                autoIncrement: true
            },
            userid: {
                type: Sequelize.TEXT,
                //unique: true
            },
            authorization: {
                type: Sequelize.INTEGER,
            }
        }, {
            timestamps: false,
        }
    )
    return Users;
}