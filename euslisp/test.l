#!/usr/bin/env roseus

(require "eus-lp.l")

(if (not (eus_lp::test-all)) (throw :eus-lp-test-exception nil))

(exit 1)
