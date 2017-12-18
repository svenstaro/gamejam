let City = artifacts.require("City");

contract('City', (accounts) => {
    it("has Cityblocks that can be claimed", () => {
        return City.deployed().then((instance) => {
            return instance.claimCityblock.call(2, -3);
        }).then((result) => {
            assert.isTrue(result);
        });
    });

    it("cannot claim same block twice", () => {
        let city;

        return City.deployed().then((instance) => {
            city = instance;
            return city.claimCityblock(2, -3);
        }).then(() => {
            return city.claimCityblock.call(2, -3);
        }).then((result) => {
            // Second result should be false;
            assert.isFalse(result);
        });

    });
});

// City city = City(DeployedAddresses.City());
//
// function testUserCanClaimCityblock() {
//     bool success = city.claimCityblock(2, -3);
//     Assert.isTrue(success, "Unable to claim Cityblock");

// int (xloc, yloc, owner) = (city.m_cityblocks(0));
// int lol = 2;
// Assert.equal(xloc, lol);
// Assert.equal(yloc, -3);
// Assert.equal(owner, msg.sender);
// }

// function testUCantClaimCityblockTwice() public {
//     bool success = city.claimCityblock(2, -3);
//     bool failure = city.claimCityblock(2, -3);
//     Assert.notEqual(success, failure);
// }
