pragma solidity ^0.4;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/City.sol";

contract TestCity {
    City city = City(DeployedAddresses.City());

    function testUserCanClaimCityblock() public {
        bool success = city.claimCityblock(2, -3);
        Assert.isTrue(success, "Unable to claim Cityblock");

        City.Cityblock memory expected = City.Cityblock({
            xloc: 2,
            yloc: -3,
            owner: msg.sender
        });
        City.Cityblock storage rofl = expected;

        Assert.equal(city.m_cityblocks(0), expected);
    }

    function testUCantClaimCityblockTwice() public {
        bool success = city.claimCityblock(2, -3);
        bool failure = city.claimCityblock(2, -3);
        Assert.notEqual(success, failure);
    }
}
