#!/usr/bin/env python3
# Copyright (c) 2020 The BitcoinSubsidium Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

"""Test validation of mined block for different periods"""


from test_framework.mininode import CBlock
from test_framework.test_framework import BitcoinSubsidiumTestFramework
from test_framework.blocktools import *
from test_framework.util import assert_equal, assert_raises_rpc_error, hex_str_to_bytes
from test_framework.script import CScript, CScriptOp
from time import sleep


def create_and_submit_first_block(node):
    block = CBlock()
    block.nVersion = 536870912
    block.hashPrevBlock = int("07a55485e188ffddb9d199debe06eab965e92a2f14c49c106705f13d4292939b", 16)
    block.hashMerkleRoot = int("7f45e89dc0b49bf1c7cb1989541ee24cef220f3a9a4f2c849fb1aa48252d52c8", 16)
    block.nTime = 1602066398
    block.nBits = int("207fffff", 16)
    block.nNonce = 0
    block.vtx.append(create_coinbase(0))
    block.vtx[0].nVersion = 2
    block.vtx[0].vin[0].scriptSig = CScript(hex_str_to_bytes("510101"))
    block.vtx[0].vout[0].scriptPubKey = CScript(hex_str_to_bytes("2102962c441d08d93d08d1c0b46610af8b9292528317b66799d62c57c10e32121ed1ac"))
    block.vtx[0].vout.append(CTxOut(0, CScript(hex_str_to_bytes("6a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf9"))))
                                 
    node.submitblock(ToHex(block))

def create_and_submit_block(node, nVersion, hashPrevBlock, hashMerkleRoot, nTime, nNonce, scriptSig):
    block = CBlock()
    block.nVersion = nVersion
    block.hashPrevBlock = int(hashPrevBlock, 16)
    block.hashMerkleRoot = int(hashMerkleRoot, 16)
    block.nTime = nTime
    block.nBits = int("207fffff", 16)
    block.nNonce = nNonce
    block.vtx.append(create_coinbase(0))
    block.vtx[0].nVersion = 1
    block.vtx[0].vin[0].scriptSig = CScript(hex_str_to_bytes(scriptSig))
    block.vtx[0].vin[0].nSequence = 0
    block.vtx[0].vout[0].scriptPubKey = CScript(hex_str_to_bytes("6a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf9"))
    block.vtx[0].vout[0].nValue = 0
    block.vtx[0].vout.append(CTxOut(0, CScript(hex_str_to_bytes("76a9145e53f01cbd5f5ffd50458cfe4a83069e193714f288ac"))))
    block.vtx[0].vout[1].nValue = 5 * COIN

    node.submitblock(ToHex(block))

def submit_x16r_blocks(node):
    create_and_submit_block(node, 536870912, "00ce049d7a5d944cb9cf54b20dea6a9b5b1ffab7f9aa85fed6be1a4ee5dd0975",
                             "4d9b84653ddac40f903493b982365a450dfc578feaad700d6bf9d6e972f2cbb6", 1602066411, 1989148672,
                             "5304eb977d5f0818000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "00784f797eafa3e97638ae29c43c6b3318a5635061f5f6f7f1a4fdd521036612",
                             "b04e538d89326bd9e547183696d263af0093f3f3b5e0118ed4585dada9ce3c15", 1602066412, 2475884544,
                             "5404ec977d5f0818000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "01698619fd66811ef10f1ba23ea3609fc978bc0a6d66561970056da281fc64e3",
                             "c88fab9846860d95be9b553eca85771a65c75d0196a0aeb0da8f5de63cd1d65f", 1602066413, 1481244672,
                             "5504ed977d5f0840000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "004cbb65f086646f8221f4346c27934dc0782fd809f3be255fe74364dc3a46d0",
                             "41ba703a46f35f64d7ae9a1d63ea34ec6406c7e6137352b36b0c398f7a74d341", 1602066436, 905514752,
                             "560404987d5f0840000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "0139fdf3482ae8e01de82afe3280b61aadc6b1d99344943116907cff93e9fada",
                             "a38dee8c8a3c72601db38f17d63e75d7df13ea9a8c4013576e500c2e019b5b72", 1602066436, 1073680128,
                             "570404987d5f0860000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "007d60fb5595e67e40db3a15c992b80d2783bc6b5caa3b47e00714f5e95a9bf6",
                             "17a2146eb4445fa67d6a22a51c452df3f3aa2f04eef8277cae2e4d9ff2cd0e33", 1602066446, 4193914624,
                             "58040e987d5f0860000000000000000d2f6e6f64655374726174756d2f")
    
    create_and_submit_block(node, 536870912, "00c4fa74f0dd871a100d2e14a1292a712775f37f10a0bb86ea92d4ca9159f9c1",
                             "7811e257bb6a8516cb2733e8370f1527006fa4e565dc42547c208f11c1558c22", 1602066447, 1241386752,
                             "59040f987d5f0860000000000000000d2f6e6f64655374726174756d2f")

def submit_x16r_maintance_blocks(node):
    create_and_submit_block(node, 536870912, "01037df2e79f0e89804f586009bc3047ae4c30e2d693631e63b7646c5a85f29e",
                             "c1475bf6bacf08662315c7e7bf01c3178da95d1cff202ac8cd5eac06df506305", 1602066447, 205127680,
                             "5a040f987d5f0868000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "0107d4f935ae3230b7b162cc7adbaaf5f4e2ebb80b1103e8ee20a66fcbcd37ac",
                             "29e670f5e655b03147064c93d0459de5a3ce3f35390a57a8ef4e9bd96df259ca", 1602066453, 218042112,
                             "5b0415987d5f0868000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "0099abc2ea0fae411a179cc080dcafc8009201ec3fa3eeb66ca37f0149a309f7",
                             "fc8f7ea0c369a1c3996554c2cb31ad846a867c9e45338894490d97b8e1c9c7c6", 1602066453, 1431437312,
                             "5c0415987d5f0870000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "01499d40f0a8c79d3fc56149a9f9f476f432820462f56c4ba608794da2d745b2",
                             "4119f846c1ad8e7372dd71775bc2234baf77d0d1ca7588d87ccb45d9dceaeb5c", 1602066463, 831389696,
                             "5d041f987d5f0870000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "008ec188354feaa0d2fa277432bd596411ba428eb4e2d78dc83f824464bc8df2",
                             "a2c8ccf5f6862d8145b54a091aebb8bec540c2ca8fd927d204c87a30533f52dd", 1602066463, 1284042752,
                             "5e041f987d5f0870000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "005e50703b2a9fc83130bb2d0928b91b54e304648c1a0bc1ad2b1b7213c259db",
                             "4ac6c14982ec2b488e79598a5306b9c100fdb5c688c962c4f26b7e0b6ecb87fa", 1602066465, 1905262592,
                             "5f0421987d5f0870000000000000000d2f6e6f64655374726174756d2f")

def submit_scrypt2_maintance_blocks(node):
    create_and_submit_block(node, 536870912, "005c4c63af85f3f991afc707e32dc6fbe7b9242f79fa1757df2bb1f37ceb18c1",
                             "0a993f04e79234f2f130d139a53cadbd5e71240a626dd699ea03b2c4c449f35a", 1602066632, 4177526655,
                             "6004c8987d5f0840000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "1e5194b838e00dea864cb30a96162fbdd51cc2e0779a8a2c8374cbbb2c6c7a0c",
                             "4b6338cc64ea6533eaf31daa146fc55e714ca5a813e3ef3c117580e325a56abc", 1602161828, 2863311370,
                             "011104a40c7f5f0818000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "a73c5e5c892ab1635730b0511f0db42adda29aaa40a5551e1bfdac012750520f",
                             "c7bef233e5ae4dbd478ed2483a50a6b4e0eb174ed45ece8624a11db2f0fd3712", 1602161888, 2695539370,
                             "011204e00c7f5f0818000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 536870912, "8a9f9c399f1de9d22179fb23ff865e1e01d7d532f20ca066a6ebbe3b099a501e",
                             "f2b948814bc56ee53aa5a800900e92e4a53b517e7af27f5275df7a375cbc5043", 1602161901, 1213552085,
                             "011304ed0c7f5f0818000000000000000d2f6e6f64655374726174756d2f")

def submit_scrypt2_blocks(node):
    create_and_submit_block(node, 1073741824, "ae25bbab4bafac5cb9c3c72d2cd933ea442f5690ddb0e8a825816c22da5f8373",
                            "e1912560c8b66850c8ecacdfd3d672c8c3005f60c806d5640a470f1ad959f2d6", 1602161910, 1280660885,
                            "011404fa0c7f5f0818000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 1073741824, "73a600249f7a4ef77f58703a7cc0d00d579c3240c0ff1c42a8f1ecbf5f59a1cf",
                             "1b43cdc216316a3bd90f420f9a0eb723dea57fdb82e18a185dcd40b49423b400", 1602161933, 1414878485,
                             "0115040d0d7f5f0818000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 1073741824, "122fb0df9260f5d8c585221cf9da6658a82fa72205423d35586cafeb14500891",
                             "6b8b2572195e0c04b0605350f16936548f00d0dce8ee786cd0977f83fa988889", 1602161970, 1247106485,
                             "011604420d7f5f0818000000000000000d2f6e6f64655374726174756d2f")

    create_and_submit_block(node, 1073741824, "9a4138f568c294d6ceb4fad2a69a472f3baebaf8f2ee9facb808dfd4d4f238cb",
                             "615153bc758cee5d5fb02bff559b366d91cbc0c38662514c0539bcae1845c725", 1602162036, 2829756970,
                             "011704850d7f5f0818000000000000000d2f6e6f64655374726174756d2f")


class MiningValidationTest(BitcoinSubsidiumTestFramework):
    def set_test_params(self):
        self.num_nodes = 3
        self.setup_clean_chain = True
        self.extra_args = [['-maxtipage=999999999'], ['-maxtipage=999999999'], ['-maxtipage=999999999', '-scrypt2peeriod=18']]

    def run_test(self):
        node = self.nodes[0]

        self.log.info('getmininginfo initial state')
        mining_info = node.getmininginfo()
        assert_equal(mining_info['blocks'], 0)
        assert_equal(mining_info['chain'], 'regtest')


        self.log.info('submit first block')
        create_and_submit_first_block(node) 
        assert_equal(int(node.getbestblockhash(), 16), int("390a60ef8a2f62f53f5f6e7c9ab96cb743e5842a009ee52d21cc43cd17650e29", 16))


        self.log.info('submit second block')
        create_and_submit_block(node, 536870912, "390a60ef8a2f62f53f5f6e7c9ab96cb743e5842a009ee52d21cc43cd17650e29",
                                "f25cac9fd823333f01e24e33646efc7c3a8ac8e3d9f0623d5ebde0f9e45ce702", 1602066399, 816775168,
                                "5204de977d5f0818000000000000000d2f6e6f64655374726174756d2f")


        self.log.info('check nodes synced')
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 2)
        assert_equal(self.nodes[1].getmininginfo()['blocks'], 2)


        self.log.info('submit invalid block')
        create_and_submit_block(node, 536870912, "00ce049d7a5d944cb9cf54b20dea6a9b5b1ffab7f9aa85fed6be1a4ee5dd0975",
                                "1d9b84653ddac40f903493b982365a450dfc578feaad700d6bf9d6e972f2cbb6", 1602066411, 1989148672,
                                "5304eb977d5f0818000000000000000d2f6e6f64655374726174756d2f")
        assert_equal(node.getmininginfo()['blocks'], 2)

        
        self.log.info('submit blocks for x16r period')
        submit_x16r_blocks(node)
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 9)
        assert_equal(self.nodes[1].getmininginfo()['blocks'], 9)
        

        self.log.info('submit x16r blocks for maintance period')
        submit_x16r_maintance_blocks(node)
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 15)
        assert_equal(self.nodes[1].getmininginfo()['blocks'], 15)
        

        self.log.info('submit scrypt2 blocks for maintance period')
        submit_scrypt2_maintance_blocks(node)
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 19)
        assert_equal(self.nodes[1].getmininginfo()['blocks'], 19)
        assert_equal(self.nodes[2].getmininginfo()['blocks'], 17) # scrypt2 period starts at block 18 on third node, no more blocks should be accepted by this node


        self.log.info('try to submit block for scrypt2 period with old version')
        create_and_submit_block(node, 536870912, "ae25bbab4bafac5cb9c3c72d2cd933ea442f5690ddb0e8a825816c22da5f8373",
                                "e1912560c8b66850c8ecacdfd3d672c8c3005f60c806d5640a470f1ad959f2d6", 1602161910, 1280660885,
                                "011404fa0c7f5f0818000000000000000d2f6e6f64655374726174756d2f")
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 19)


        self.log.info('submit blocks for scrypt2 period')
        submit_scrypt2_blocks(node)
        sleep(1)
        assert_equal(node.getmininginfo()['blocks'], 23)
        assert_equal(self.nodes[1].getmininginfo()['blocks'], 23)
        assert_equal(self.nodes[2].getmininginfo()['blocks'], 17)


if __name__ == '__main__':
    MiningValidationTest().main()