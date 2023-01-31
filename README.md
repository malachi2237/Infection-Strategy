# Infection Strategy - In Progress
This is a work-in-progress turn-based strategy game project in Unreal Engine 5.0. It is a small project created from the theme of "infectious". I have two goals for the project:
* To design and implement gameplay systems in the Unreal Engine and to modify gameplay with feedback
* To take a deeper dive into Unreal C++ by primarily using C++ to implement features. This includes learning best practices for programming with Unreal.
## Overview
### Premise
You are at war with a rival faction. Fortunately, your faction has created a revolutionary gas that heals and boosts your own people and equipment while damaging those of your enemies. Unfortunately, so has your enemy.
Face off in a battle with your enemy by taking control of a squad of vehicles and by strategically distributing gas in the area.
### Gameplay
Players begin with a set of vehicles and an area of influence for their gas. At each turn, a player can move their vehicles up to certain amount of tiles, attack with a vehicle, or extend the area of their gas. These actions can be performed in any order.

Gas can have four different levels of concentration on a tile. When a gassed tile is connected to a friendly system of gas, its concentration depends on the amount of adjacent gassed tiles. Otherwise the concentration diminishes each turn.
Each attack does varying amounts of vehicle and gas damage. Attacks affecting gas decrease the concentration level of a tile. Gas provides buffs to friendly units and debuffs to enemy units whenever they are on a gassed tile.
At the end of a turn. Units are also healed or damaged by friendly or enemy gas.

The game ends when a player no longer has any units.
## Progress
Basic UI and turn-based and tile systems are implemented. The groundwork of gas and vehicles are also laid.

## Closing
This project has already taught me much about developing in Unreal using C++, and I look forward to learning more. This document will be updated when changes occur and progress is made.
