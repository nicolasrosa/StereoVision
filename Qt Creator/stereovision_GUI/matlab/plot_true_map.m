clc
clear all
close all

truemap = textread('truemap.txt');
% truemap = textread('truemap_teddy.txt');
% truemap = textread('truemap_chair.txt');



figure,surf(truemap);
colormap(jet)
title('TrueMap')
xlabel('x')
ylabel('y')