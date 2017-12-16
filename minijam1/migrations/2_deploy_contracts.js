var City = artifacts.require("./City.sol");

module.exports = function(deployer) {
  deployer.deploy(City);
};
