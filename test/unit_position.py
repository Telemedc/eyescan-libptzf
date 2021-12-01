import os
import sys
import unittest

PYMOD_PATH = os.environ['PYMOD_PATH']
sys.path.insert(0, PYMOD_PATH)

import pyptzf

class TestPosition(unittest.TestCase):
    KEY_STRINGS = ('x', 'y', 'z', 'f')
    KEY_ENUMS = (
        pyptzf.Position.Key.x,
        pyptzf.Position.Key.y,
        pyptzf.Position.Key.z,
        pyptzf.Position.Key.f,
    )

    def test_members(self):
        p = pyptzf.Position(1,2,3,4)
        self.assertAlmostEqual(p.x, 1)
        self.assertAlmostEqual(p.y, 2)
        self.assertAlmostEqual(p.z, 3)
        self.assertAlmostEqual(p.f, 4)

        # test readwrite
        for k in TestPosition.KEY_STRINGS:
            with self.subTest(f"readwrite: {k}"):
                setattr(p, k, 3.14)
                self.assertAlmostEqual(getattr(p, k), 3.14)

    def test_keys(self):
        p = pyptzf.Position()
        for k in TestPosition.KEY_ENUMS:
            with self.subTest(k.name):
                self.assertIn(k, p.keys())
        self.assertEqual(len(p.keys()), 4)

    def test_operators(self):
        pass

    def test_getitem(self):
        p = pyptzf.Position(1,2,3,4)
        for k in pyptzf.Position.keys():
            with self.subTest(f"with enum member: {k}"):
                self.assertEqual(getattr(p, k.name), p[k])
        # prefer using enums if possible in actual usage
        # (probably very slightly more effecient)
        for k in pyptzf.Position.keys():
            with self.subTest(f"with string: {k.name}"):
                self.assertEqual(getattr(p, k.name), p[k.name])
        with self.assertRaises(KeyError):
            p['potato']

    def test_contains(self):
        p = pyptzf.Position()
        for s in TestPosition.KEY_STRINGS:
            self.assertTrue(pyptzf.Position.__contains__(s))
            self.assertIn(s, p)
        for e in TestPosition.KEY_ENUMS:
            self.assertTrue(pyptzf.Position.__contains__(e))
            self.assertIn(e, p)
        self.assertFalse(pyptzf.Position.__contains__("potato"))
        self.assertFalse('potato' in p)

    def test_str(self):
        p = pyptzf.Position(1,2,3,4)
        self.assertEqual(
            str(p),
            "G0 X1.000 Y2.000 Z3.000 E4.000",
        )
        self.assertEqual(
            p.gcode,
            "G0 X1.000 Y2.000 Z3.000 E4.000",
        )

    def test_json(self):
        p = pyptzf.Position(1,2,3,4)
        p2 = pyptzf.Position.from_json(p.to_json())
        self.assertEqual(p, p2)
        with self.assertRaises(pyptzf.NlohmannJsonError):
            pyptzf.Position.from_json("potato")


if __name__ == "__main__":
    unittest.main()
