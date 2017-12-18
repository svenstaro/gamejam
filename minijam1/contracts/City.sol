pragma solidity ^0.4;

contract City {
    struct Cityblock {
        // xloc and yloc are locations relative to the center which is itself
        // at (0, 0).
        int xloc;
        int yloc;
        address owner;
    }

    Cityblock[] public m_cityblocks;

    function getCityblocks() public returns (Cityblock[]) {
        return m_cityblocks;
    }

    function claimCityblock(int xloc, int yloc) public returns (bool) {
        // Check that this block isn't claimed yet.
        // Sadly we'll have to iterate over all the Cityblocks here because
        // mappings are not powerful enough.
        for (uint i = 0; i < m_cityblocks.length; i++) {
            if (m_cityblocks[i].xloc == xloc && m_cityblocks[i].yloc == yloc) {
                // Early exit if the CityBlock is already claimed.
                return false;
            }
        }
        // If the block isn't claimed yet, assign it to the user.
        m_cityblocks.push(Cityblock({
            xloc: xloc,
            yloc: yloc,
            owner: msg.sender
        }));
        return true;
    }
}
