# Enterprise Development Baseline

The teaching repository adopts a lightweight product-development baseline before the enterprise capstone.

## Change control

- One issue or lab goal per branch and commit series.
- Pull-request-sized changes even when working alone.
- No direct changes to main after the bootstrap phase; integrate through reviewed commits.
- Record architecture decisions in docs/adr/.

## Safety and security

- Never commit credentials, production keys, device unlock data, or private logs.
- Treat bootloader, Flash erase, TrustZone attribution, and firmware update operations as safety-sensitive.
- Validate image address, size, CRC/Hash, version, and rollback metadata before changing a bootable image.
- Keep debug unlock and mass-erase procedures documented and explicit.

## Quality gates

- Clean build from a documented command.
- Static checks where available.
- Unit or host-side tests for pure logic.
- Hardware verification for board behavior.
- Review of the diff, generated files, and dependency direction.
- Handoff update with exact evidence and known limitations.

## Release evidence

Each release records tool versions, source commit, configuration source, binary checksum, board revision, test result, and known issues. A successful compile is not a hardware release.
