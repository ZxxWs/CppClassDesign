/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80015
 Source Host           : localhost:3306
 Source Schema         : xfgl

 Target Server Type    : MySQL
 Target Server Version : 80015
 File Encoding         : 65001

 Date: 07/05/2020 11:49:38
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for grade19070041
-- ----------------------------
DROP TABLE IF EXISTS `grade19070041`;
CREATE TABLE `grade19070041`  (
  `Snum` char(11) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生学号',
  `Sname` char(8) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生姓名',
  `Sgrade` char(9) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生的班级',
  `Sscore` double(255, 1) NOT NULL DEFAULT 0.0 COMMENT '学分总分',
  `Sdetail` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '学生的得分细则。格式：#事件#时间#分数。',
  `Sremark` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '学生的备注',
  PRIMARY KEY (`Snum`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of grade19070041
-- ----------------------------
INSERT INTO `grade19070041` VALUES ('1907004126', '周一', '19070041', 7.0, '#蓝桥杯#2018.04#4.0%#计设大赛#2019.05#3.0%', '');

-- ----------------------------
-- Table structure for gradeempty
-- ----------------------------
DROP TABLE IF EXISTS `gradeempty`;
CREATE TABLE `gradeempty`  (
  `Snum` char(11) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生学号',
  `Sname` char(8) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生姓名',
  `Sgrade` char(9) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生的班级',
  `Sscore` double(255, 1) NOT NULL DEFAULT 0.0 COMMENT '学分总分',
  `Sdetail` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '学生的得分细则。格式：#事件#时间#分数%',
  `Sremark` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '学生的备注',
  PRIMARY KEY (`Snum`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for gradelist
-- ----------------------------
DROP TABLE IF EXISTS `gradelist`;
CREATE TABLE `gradelist`  (
  `Grade` char(9) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`Grade`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gradelist
-- ----------------------------
INSERT INTO `gradelist` VALUES ('19070041');

-- ----------------------------
-- Table structure for userpass
-- ----------------------------
DROP TABLE IF EXISTS `userpass`;
CREATE TABLE `userpass`  (
  `pass` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of userpass
-- ----------------------------
INSERT INTO `userpass` VALUES ('1');

SET FOREIGN_KEY_CHECKS = 1;
