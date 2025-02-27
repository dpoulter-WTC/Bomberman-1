#include "../include/IEnemy.hpp"

#include <iostream>

const float AUTOSWITCH = 8;
const sf::Vector2f DEFAULT_START(11.5, 11.5);
const float DEFAULT_SPEED = 2.2;
const int NUM_MOVEMENT_STATES = 4;
const float AGGROTIME = 8;

// Testing order here is important
// Test cardinals first

IEnemy::IEnemy() :
					_position(DEFAULT_START),
					_enemySpeed(DEFAULT_SPEED),
					moveState(EnemyMoveState::north),
					_switchTime(AUTOSWITCH),
					appears({1,2}),
					type(EnemyType::EGeneric),
					_wallPass(false)
{
}

IEnemy::IEnemy(sf::Vector2f start):
					_position(start),
					_enemySpeed(DEFAULT_SPEED),
					moveState(EnemyMoveState::north),
					_switchTime(AUTOSWITCH),
					appears({1,2}),
					type(EnemyType::EGeneric),
					_wallPass(false)
{
}

IEnemy::~IEnemy()
{
	return;
}

bool IEnemy::correctEnemyCellCollision(sf::Vector2i cell)
{
	const float RADIUS = 0.5;

	// Use variables to avoid multiple pointer refs
	float enemyX = this->_position.x;
	float enemyY = this->_position.y;
	int cellX = cell.x;
	int cellY = cell.y;

	float testX = enemyX;
	float testY = enemyY;

	// Check for the closest edge
	if (enemyX < cellX)
		testX = cellX; // Left edge
	else if (enemyX > cellX + 1)
		testX = cellX + 1; // Right edge
	if (enemyY < cellY)
		testY = cellY; // Top edge
	else if (enemyY > cellY + 1)
		testY = cellY + 1; // Bottom edge

	// Evaluate distance from closest edges
	sf::Vector2f diff(enemyX - testX, enemyY - testY);

	// Pythag distance needed for corners
	float distance = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));

	// If the distance is less than the radius, collision!
	if (distance <= RADIUS - 0.0001)
	{
		if (diff.y == 0)
		{
			if (enemyX > cellX)
				this->_position.x += RADIUS - diff.x;
			else
				this->_position.x -= RADIUS + diff.x;
		}
		else if (diff.x == 0)
		{
			if (enemyY > cellY)
				this->_position.y += RADIUS - diff.y;
			else
				this->_position.y -= RADIUS + diff.y;
		}
		else
		{
			sf::Vector2f corner(testX, testY);
			sf::Vector2f correction = (diff / distance) * RADIUS;
			this->_position = corner + correction;
		}
		return true;
	}
	return false;
}

void IEnemy::changeMoveState()
{
	this->moveState = (EnemyMoveState)(rand() % (NUM_MOVEMENT_STATES));
}

void IEnemy::update(float deltaTime, const Map &map)
{
	_switchTime -= deltaTime;
	if(_switchTime <= 0)
	{
		changeMoveState();
		_switchTime = (rand() % (int)AUTOSWITCH)+1;
	}
	move(deltaTime, map);
}
void IEnemy::changeAggression()
{
	return;
};
void IEnemy::move(float deltaTime, const Map &map)
{
	sf::Vector2i movement(0, 0);
	switch (this->moveState)
	{
		case EnemyMoveState::east:
			movement.x += 1;
			break;
		case EnemyMoveState::west:
			movement.x -= 1;
			break;
		case EnemyMoveState::north:
			movement.y -= 1;
			break;
		case EnemyMoveState::south:
			movement.y += 1;
			break;
		default: 
			break;
	}
	this->_position = this->_position + (sf::Vector2f(movement) * this->_enemySpeed * deltaTime);
	sf::Vector2i enemyCell(this->_position);
	int i = -1;
	for (sf::Vector2i direction : TEST_NEIGHBOURS)
	{
		i++;
		sf::Vector2i cell = enemyCell + direction;
		Tile tile = map.tileAt(cell);
		if (!(tile == Tile::Clear || (_wallPass && (tile == Tile::Destructible || tile == Tile::Bomb))))
		{
			if (correctEnemyCellCollision(cell) && i < 4)
			// Only change if a collision is detected on a cardinal direction. 
				changeMoveState();
		}
	}
}

const sf::Vector2f &IEnemy::position() const
{
	return this->_position;
};