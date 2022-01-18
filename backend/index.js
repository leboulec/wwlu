const express = require('express')
const cors = require('cors')

const app = express();

var corsOption = {
    origin: "*"
};

app.use(cors(corsOption));
app.use(express.json());

// parse requests of content-type - application/x-www-form-urlencoded
app.use(express.urlencoded({ extended: true }));

const db = require("./app/models");
db.sequelize.sync();

require("./app/routes/")(app);
require("./app/routes/times.route")(app);
require("./app/routes/users.route")(app);
require("./app/routes/open.route")(app);
//require("./app/routes/checksConfig.route")(app);
//require("./app/routes/index")(app);

// set port, listen for requests
const PORT = process.env.PORT || 8081;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}.`);
});