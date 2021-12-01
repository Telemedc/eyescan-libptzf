import os
import sys
import unittest

PYMOD_PATH = os.environ['PYMOD_PATH']
sys.path.insert(0, PYMOD_PATH)

import pyptzf


class TestControllerConfig(unittest.TestCase):

    def test_operators(self):
        # operator __eq__ and __ne__
        device = "/dev/foo"
        min = pyptzf.Position(0,0,0,0)
        max = pyptzf.Position(1,1,1,1)
        do_connect = False
        c1 = pyptzf.Controller.Config(device, min, max, do_connect)
        c2 = pyptzf.Controller.Config(device, min, max, do_connect)
        c3 = pyptzf.Controller.Config(device, min, max, True)
        self.assertEqual(c1, c2)
        self.assertNotEqual(c1, c3)

    def test_is_valid_position(self):
        device = "/dev/foo"
        min = pyptzf.Position(0,0,0,0)
        max = pyptzf.Position(1,1,1,1)
        do_connect = False

        c = pyptzf.Controller.Config(device, min, max, do_connect)
        p = pyptzf.Position()  # 0,0,0,0

        self.assertTrue(c.is_valid_position(min))
        self.assertTrue(c.is_valid_position(max))
        self.assertTrue(c.is_valid_position(p))

        offsets = (
            pyptzf.Position(1,0,0,0),
            pyptzf.Position(0,1,0,0),
            pyptzf.Position(0,0,1,0),
            pyptzf.Position(0,0,0,1),
        )

        for offset in offsets:
            self.assertFalse(c.is_valid_position(min - offset))
            self.assertFalse(c.is_valid_position(max + offset))


if __name__ == "__main__":
    unittest.main()
