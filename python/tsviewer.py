#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# vim: fenc=utf-8 ts=4 sw=4 sts=4 ft=python autoindent expandtab

import argparse
import pprint
import pathlib
import json


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f",
                        "--file",
                        type=str,
                        required=True,
                        help="file to read")
    parser.add_argument("-l",
                        "--line",
                        type=int,
                        default=3,
                        help="show X line")
    parser.add_argument("-c",
                        "--column",
                        type=str,
                        help="only show column x,y,z")
    args = parser.parse_args()

    if not pathlib.Path(args.file).exists():
        parser.error(f"file {args.file} is not exist")

    return args


def filter_map(map, array):
    new_map = {}
    for k in map:
        if k in array:
            new_map[k] = map[k]
    return new_map


def main():
    args = parse_args()
    file_name = args.file
    column = None
    if args.column is not None:
        column = args.column.split(",")

    with open(file_name) as fp:
        num = 0
        header = None
        for line in fp:
            num += 1
            segment = line.split()
            if num == 1:
                header = segment
                continue
            else:
                output = dict(zip(header, segment))
                if column is not None:
                    output = filter_map(output, column)
                pprint.pprint(json.dumps(output))
            if num == (args.line + 1):
                return


if __name__ == '__main__':
    main()
