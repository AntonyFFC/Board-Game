#pragma once

#include "Equipment.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <string>

class Pawns;

enum class RangedAttackPhase {
    LungeOut,
    LungeInAndProjectile,
    Finishing
};

class AttackSystem
{
public:
    explicit AttackSystem(Pawns* pawns);

    void attack(int pawnNum, int attackedNum, Equipment* weapon);
    void update(float dt);
    void draw(sf::RenderTarget& target);
    bool isActive() const;
    bool isAnimatingPawn(const class Pawn* pawn) const;

private:
    bool hasEnoughActions(class Pawn* attacker, Equipment* weapon) const;
    int calculateAttackValue(class Pawn* attacker, Equipment* weapon) const;
    void resolveMeleeAttack(class Pawn* attacker, class Pawn* attacked,
        Equipment* weapon, int attackValue);
    void startRangedAttack(int pawnNum, int attackedNum, Equipment* weapon,
        int attackValue, int missMax);
    void startLungeOut(class Pawn* attacker, class Pawn* attacked, sf::Vector2f homePos);
    void spawnProjectile(sf::Vector2f startPos, sf::Vector2f targetPos,
        const std::string& weaponName);
    void startLungeIn(class Pawn* attacker, sf::Vector2f lungePos, sf::Vector2f homePos);
    void resolveRangedImpact();
    float facingAngleToward(sf::Vector2f from, sf::Vector2f to) const;
    sf::Vector2f pawnScreenPosition(class Pawn* pawn) const;
    const sf::Texture& projectileTextureFor(const std::string& weaponName) const;
    bool isSlingshot(const std::string& weaponName) const;

    struct PendingRangedAttack {
        int attackerNum;
        int attackedNum;
        Equipment* weapon;
        int attackValue;
        int missMax;
        sf::Vector2f homePos;
        sf::Vector2f lungePos;
        RangedAttackPhase phase;
        bool impactResolved = false;
        std::unique_ptr<Projectile> projectile;
    };

    Pawns* pawns;
    std::optional<PendingRangedAttack> activeRangedAttack;
    sf::Texture arrowTexture;
    sf::Texture rockTexture;
    bool texturesLoaded = false;
};
